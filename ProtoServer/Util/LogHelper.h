#pragma once

#include <spdlog\spdlog.h>


class LogHelper
{
public:
	static LogHelper* GetInstance();

	shared_ptr<spdlog::logger>& GetConsoleLogger();

private:
	static LogHelper s_instance;;

	LogHelper();

	shared_ptr<spdlog::logger> _consoleLog;
};
