#pragma once
#include "common.h"

/* @TODO use printf */

enum LogLevel { NONE, ERROR, DEBUG };

void Log(const std::string& msg, LogLevel&& logLevel = LogLevel::DEBUG, const std::string& origin = "");

void Log(const glm::vec2& v, const std::string& name = "");
void Log(const glm::vec3& v, const std::string& name = "");

void Log(const float& val, const std::string& name = "");

void Log(const char* val, const std::string& name);
