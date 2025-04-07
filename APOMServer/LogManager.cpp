
#include "pch.h"
#include "LogManager.h"

LogManager::LogManager() noexcept
{
	//===================================================
	// ���� �ð��� ������� �α׸� ����ϴ� ���� ����
	//===================================================

	// ���� �ð��� ������
	time_t now = time(nullptr);
	struct tm timeInfo;
	localtime_s(&timeInfo, &now);  // ���� �ð��� ����ü�� ����

	// �ð� ������ ������� ���ϸ��� ����
	std::ostringstream oss;
	oss << "server_performance_log_";
		//<< std::put_time(&timeInfo, "%Y-%m-%d_%H-%M-%S")  // ��¥�� �ð��� "YYYY-MM-DD_HH-MM-SS" ��������
		//<< ".txt";

	std::string fileName = oss.str();

	// �α� ���� ����
	m_logFile.open(fileName, std::ios::out | std::ios::app);
	if (!m_logFile.is_open()) {
		std::cerr << "Failed to open log file: " << fileName << std::endl;
	}
}

LogManager::~LogManager() noexcept
{
    // �α� ���� �ݱ�
    if (m_logFile.is_open()) {
        m_logFile.close();
    }
}

void LogManager::SetLogLevel(LogLevel level)
{
    currentLogLevel = level;
}

void LogManager::saveLog(void)
{
	// �α� ���� �ݱ�
	if (m_logFile.is_open()) {
		for (int i = 0; i < logQueue.rear; ++i)
			m_logFile << logQueue.queue[i] << std::endl;

		logQueue.rear = 0;
	}
}

std::string LogManager::GetCurrentTime(void)
{
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    std::tm tm_now;

    // ������ ������ ������� ���� �ð� ��ȯ (POSIX: localtime_r, Windows: localtime_s)
#ifdef _WIN32
    localtime_s(&tm_now, &time_t_now);
#else
    localtime_r(&time_t_now, &tm_now);
#endif

    std::ostringstream oss;
    oss << std::put_time(&tm_now, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}
