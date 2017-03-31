#include "stdafx.h"
#include "LogHelper.h"


LogHelper::LogHelper()
{
	_consoleLog = spdlog::stdout_color_mt("ProtoServer");
}

shared_ptr<spdlog::logger>& LogHelper::GetConsoleLogger()
{
	return _consoleLog;
}


