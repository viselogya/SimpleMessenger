#include "pch.h"
#include "Logger.h"


Logger& Logger::Instance() {
	static Logger instance;
	return instance;
}

void Logger::InizializePath(const std::filesystem::path& logFilePath) {
	m_logOfstream.open(logFilePath);
}


Logger::~Logger() {
	try
	{
		m_logOfstream.close();
	}
	catch (const std::exception& ex)
	{
		std::cerr << std::format("Logger stream cannot close! GLE: {}", ex.what());
	}
}

void Logger::Debug(const std::string& text) noexcept {
	std::string formatedText = LogMessage(LogLevel::Debug, text);

	std::scoped_lock lock(m_mutexLogger);
	m_logOfstream << formatedText;
	m_logOfstream.flush();
}

void Logger::Info(const std::string& text) noexcept {
	std::string formatedText = LogMessage(LogLevel::Info, text);

	std::scoped_lock lock(m_mutexLogger);
	m_logOfstream << formatedText;
	m_logOfstream.flush();
}

void Logger::Warn(const std::string& text) noexcept {
	std::string formatedText = LogMessage(LogLevel::Warn, text);

	std::scoped_lock lock(m_mutexLogger);
	m_logOfstream << formatedText;
	m_logOfstream.flush();
}

void Logger::Error(const std::string& text) noexcept {
	std::string formatedText = LogMessage(LogLevel::Error, text);

	std::scoped_lock lock(m_mutexLogger);
	m_logOfstream << formatedText;
	m_logOfstream.flush();
}

void Logger::Fatal(const std::string& text) noexcept {
	std::string formatedText = LogMessage(LogLevel::Fatal, text);

	std::scoped_lock lock(m_mutexLogger);
	m_logOfstream << formatedText;
	m_logOfstream.flush();
}

std::string Logger::LogMessage(LogLevel logLevel, const std::string& text) const {
	std::string logLevelString;
	switch (logLevel)
	{
	case Logger::LogLevel::Debug:
		logLevelString = "Debug";
		break;
	case Logger::LogLevel::Info:
		logLevelString = "Info";
		break;
	case Logger::LogLevel::Warn:
		logLevelString = "Warn";
		break;
	case Logger::LogLevel::Error:
		logLevelString = "Error";
		break;
	case Logger::LogLevel::Fatal:
		logLevelString = "Fatal";
		break;
	}

	auto currentTime = std::chrono::zoned_time{ std::chrono::current_zone(), std::chrono::system_clock::now() }.get_local_time();

	auto threadId = std::this_thread::get_id();
	std::ostringstream oss;
	oss << threadId;
	std::string threadIdStr = oss.str();

	return FormatString(logLevelString, currentTime, text, threadIdStr);
}
