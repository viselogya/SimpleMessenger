#include "pch.h"
#include "ThreadPool.h"
#include "../Logger/Logger.h"


ThreadPool::ThreadPool(size_t numThreads)
{
    m_threads.reserve(numThreads);

    for (size_t i = 0; i < numThreads; ++i)
    {
        m_threads.emplace_back(&ThreadPool::ThreadWork, this);
    }

    Logger::Instance().Debug("ThreadPool was created");
}


ThreadPool::~ThreadPool()
{
    {
        // Сигнализируем потокам, что пора остановиться
        std::scoped_lock lock(m_mutexQueue);
        m_stop = true;
    }

    m_cv.notify_all();

    // Дожидаемся завершения всех потоков
    for (auto& th : m_threads)
    {
        if (th.joinable())
        {
            th.join();
        }
    }

    Logger::Instance().Debug("ThreadPool destroyed");
}


void ThreadPool::ThreadWork()
{
    while (true)
    {
        Task task;

        {
            // Захватываем очередь
            std::unique_lock lock(m_mutexQueue);

            // Ждём, пока очередь не станет непустой или пока не пора завершаться
            m_cv.wait(lock, [this] {
                return !m_tasks.empty() || m_stop.load();
            });

            if (m_stop && m_tasks.empty())
            {
                // Если пришёл сигнал остановки и нет задач, завершаем поток
                return;
            }

            // Забираем задачу из очереди
            task = std::move(m_tasks.front());
            m_tasks.pop();
        }

        // Перед выполнением помечаем, что задача начала выполняться
        {
            std::scoped_lock lock(m_mutexWait);
            m_executableTasks.insert(task.task_id);
        }

        Logger::Instance().Debug(std::format("start doing the task: {}", task.task_id));

        // Выполняем задачу
        try
        {
            (*task.taskFunc)();
        }
        catch (const std::exception& e)
        {
            Logger::Instance().Error(std::format("Exception in task {}! GLE: {}", task.task_id, e.what()));
        }
        catch (...)
        {
            Logger::Instance().Error(std::format("Unknown exception in task {}", task.task_id));

            throw;
        }

        // По завершении отмечаем задачу как выполненную
        MarkTaskCompleted(task);

        Logger::Instance().Debug(std::format("finished the task: {}", task.task_id));

        // Уведомляем потоки, которые могут ждать завершения
        m_cvTaskReady.notify_all();
    }
}


void ThreadPool::MarkTaskCompleted(const Task& task)
{
    // Защищаем доступ к множествам
    std::scoped_lock lock(m_mutexWait);

    // Переносим задачу из исполняемых в выполненные
    m_executableTasks.erase(task.task_id);
    m_completedTasks.insert(task.task_id);
}


void ThreadPool::Wait(uint64_t taskId)
{
    // Ожидаем, пока задача с данным ID не окажется в выполненных
    std::unique_lock lock(m_mutexWait);

    m_cvTaskReady.wait(lock, [this, taskId] {
        return m_completedTasks.contains(taskId);
    });

    // Если хотим, чтобы повторный Wait(taskId) снова ждал, удаляем ID из completed
    m_completedTasks.erase(taskId);
}


void ThreadPool::WaitAll() noexcept
{
    // Ждём, пока все исполняющиеся задачи завершатся и очередь будет пуста
    std::unique_lock lock(m_mutexWait);

    m_cvTaskReady.wait(lock, [this] {
        // Для проверки очереди нужно взять другой мьютекс, чтобы прочитать m_tasks
        std::scoped_lock lockQueue(m_mutexQueue);

        return m_executableTasks.empty() && m_tasks.empty();
    });

    // После этого можно очистить множество выполненных задач
    m_completedTasks.clear();
}
