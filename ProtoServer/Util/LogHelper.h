#pragma once

#include <spdlog\spdlog.h>


class LogHelper
{
public:
	static LogHelper Instance;

	LogHelper();
	
	shared_ptr<spdlog::logger>& GetConsoleLogger();

private:
	shared_ptr<spdlog::logger> m_consoleLog;
};
