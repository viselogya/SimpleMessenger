#pragma once

class ThreadPool
{
private:
    struct Task
    {
        std::shared_ptr<std::function<void()>> taskFunc;
        uint64_t task_id;
    };

public:
    /// Создаём пул потоков с numThreads рабочими потоками
    explicit ThreadPool(size_t numThreads);

    /// Деструктор - корректно останавливает потоки и освобождает ресурсы
    ~ThreadPool();

    /// Добавить задачу в пул.
    /// Возвращает ID задачи, по которому затем можно ждать её завершения.
    template<typename Func, typename ...Args>
    uint64_t AddTask(Func&& func, Args&&... args);

    /// Дождаться конкретной задачи по её ID.
    /// После выхода из этого метода данная задача будет удалена из множества завершённых.
    void Wait(uint64_t taskId);

    /// Дождаться выполнения всех задач, которые уже были добавлены (очередь + исполняющиеся).
    /// После выхода из этого метода множество завершённых задач очищается.
    void WaitAll() noexcept;

private:
    /// Основная рабочая функция потока
    void ThreadWork();

    /// Пометка задачи как выполненной
    void MarkTaskCompleted(const Task& task);

private:
    // --- Защита очереди ---
    std::mutex m_mutexQueue;
    std::queue<Task> m_tasks;
    std::condition_variable m_cv;

    // --- Защита статусов задач ---
    std::mutex m_mutexWait;
    std::condition_variable m_cvTaskReady;
    std::unordered_set<uint64_t> m_completedTasks;
    std::unordered_set<uint64_t> m_executableTasks;

    // --- Потоки и управление ---
    std::vector<std::thread> m_threads;
    std::atomic<bool> m_stop{ false };

    // --- ID для новых задач ---
    std::atomic<uint64_t> m_lastTaskId{ 0 };
};

/// Реализация шаблонного метода AddTask
template<typename Func, typename ...Args>
uint64_t ThreadPool::AddTask(Func&& func, Args&&... args)
{
    // Формируем задачу
    auto taskPtr = std::make_shared<std::function<void()>>(
        std::bind(std::forward<Func>(func), std::forward<Args>(args)...)
    );

    // Генерируем новый ID (простой инкремент)
    // Можно использовать fetch_add(1) + 1, чтобы начинать с 1
    uint64_t newTaskId = m_lastTaskId.fetch_add(1, std::memory_order_relaxed) + 1;

    // Добавляем задачу в очередь
    {
        std::scoped_lock lock(m_mutexQueue);
        m_tasks.push({ taskPtr, newTaskId });
    }

    // Будим один поток, чтобы он взялся за выполнение
    m_cv.notify_one();

    return newTaskId;
}
