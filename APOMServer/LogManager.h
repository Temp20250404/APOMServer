#pragma once

#include "Singleton.h"
#include <fstream>
#include <sstream>
#include <chrono>

#include "CircularQueue.h"

// �α� ���� ����
enum LogLevel {
    dfLOG_LEVEL_DEBUG = 0,
    dfLOG_LEVEL_ERROR,
    dfLOG_LEVEL_SYSTEM
};

// �α� �Ŵ��� Ŭ����
class LogManager : public SingletonBase<LogManager> {
private:
    friend class SingletonBase<LogManager>;

public:
    explicit LogManager() noexcept;
    ~LogManager() noexcept;

    // ���� �����ڿ� ���� �����ڸ� �����Ͽ� ���� ����
    LogManager(const LogManager&) = delete;
    LogManager& operator=(const LogManager&) = delete;

public:
    // ���� �α� ���� ����
    void SetLogLevel(LogLevel level);

    // �α� �޽��� �ۼ� �Լ�
    template<typename... Args>
    void LogMessage(LogLevel level, const std::string& levelStr, Args... args);

    // �α� ��� �Լ�
    template<typename... Args>
    void LogDebug(Args... args);

    template<typename... Args>
    void LogError(Args... args);

    template<typename... Args>
    void LogSystem(Args... args);

public:
    void saveLog(void);


private:
    LogLevel currentLogLevel = dfLOG_LEVEL_DEBUG; // �⺻ �α� ����
    //LogLevel currentLogLevel = dfLOG_LEVEL_ERROR; // �⺻ �α� ����

private:
    // ��� ����
    std::ofstream m_logFile;    // ���Ϳ� FPS �����͸� ����� ����

    // Ÿ�ӽ����� �Լ� (std::chrono ���)
    std::string GetCurrentTime(void);

    CircularQueue<std::string> logQueue;
};

template<typename ...Args>
inline void LogManager::LogMessage(LogLevel level, const std::string& levelStr, Args ...args)
{
    //if (level >= currentLogLevel) {
    //    std::ostringstream oss;
    //    // ���� ���ڵ��� ��Ʈ���� �߰�
    //    (oss << ... << args); // Fold expression (C++17)

    //    logQueue.enqueue(oss.str());

    //    //if (m_logFile.is_open()) {
    //    //    m_logFile << "[" << GetCurrentTime() << "][" << levelStr << "] " << oss.str() << std::endl;
    //    //    m_logFile.close();
    //    //}
    //}
}

template<typename ...Args>
inline void LogManager::LogDebug(Args ...args)
{
    do {
        LogMessage(dfLOG_LEVEL_DEBUG, "dfLOG_LEVEL_DEBUG", args...);
    } while (0);
}

template<typename ...Args>
inline void LogManager::LogError(Args ...args)
{
    do {
        LogMessage(dfLOG_LEVEL_ERROR, "dfLOG_LEVEL_ERROR", args...);
    } while (0);
}

template<typename ...Args>
inline void LogManager::LogSystem(Args ...args)
{
    do {
        LogMessage(dfLOG_LEVEL_SYSTEM, "dfLOG_LEVEL_SYSTEM", args...);
    } while (0);
}