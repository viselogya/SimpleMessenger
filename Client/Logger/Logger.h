#pragma once

class Logger
{
private:
	enum class LogLevel {
		Debug,
		Info,
		Warn,
		Error,
		Fatal
	};

public:

	static Logger& Instance();

	void InizializePath(const std::filesystem::path& logFilePath);

	~Logger();

	void Debug(const std::string& text) noexcept;

	void Info(const std::string& text) noexcept;

	void Warn(const std::string& text) noexcept;

	void Error(const std::string& text) noexcept;

	void Fatal(const std::string& text) noexcept;

private:

	template<typename... Args>
	std::string FormatString(Args&&... args) const {

		return std::vformat(m_defaultFormatString, std::make_format_args(args...));
	}

	std::string LogMessage(LogLevel logLevel, const std::string& text) const;

private:

	std::ofstream m_logOfstream;

	std::mutex m_mutexLogger;

	const std::string m_defaultFormatString = "LogLevel: {0}, ThreadId: {3}, Date: {1:%F} Time: {1:%T}, Text: {2}\n";
};

