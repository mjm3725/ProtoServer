#include "stdafx.h"
#include "LogHelper.h"


LogHelper LogHelper::s_instance;

LogHelper::LogHelper()
{
	m_consoleLog = spdlog::stdout_color_mt("ProtoServer");
}


LogHelper* LogHelper::GetInstance()
{
	return &s_instance;
}

shared_ptr<spdlog::logger>& LogHelper::GetConsoleLogger()
{
	return m_consoleLog;
}


