#pragma once

#include <spdlog\spdlog.h>

class LogHelper
{
public:
	static LogHelper* GetInstance()
	{
		static LogHelper instance;
		return &instance;
	}

	shared_ptr<spdlog::logger>& GetConsoleLogger();

private:
	LogHelper();

	shared_ptr<spdlog::logger> _consoleLog;
};

