#include "stdafx.h"
#include "LogHelper.h"


LogHelper LogHelper::s_instance;

LogHelper::LogHelper()
{
	_consoleLog = spdlog::stdout_color_mt("ProtoServer");
}


LogHelper* LogHelper::GetInstance()
{
	return &s_instance;
}

shared_ptr<spdlog::logger>& LogHelper::GetConsoleLogger()
{
	return _consoleLog;
}


