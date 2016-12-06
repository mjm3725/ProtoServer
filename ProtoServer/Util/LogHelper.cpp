#include "stdafx.h"
#include "LogHelper.h"


LogHelper LogHelper::Instance;

LogHelper::LogHelper()
{
	m_consoleLog = spdlog::stdout_color_mt("ProtoServer");
}

shared_ptr<spdlog::logger>& LogHelper::GetConsoleLogger()
{
	return m_consoleLog;
}


