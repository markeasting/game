
#include "util/log.h"

void Log(const std::string& msg, LogLevel&& logLevel, const std::string& origin) {

    std::string logLevelString;

    switch(logLevel) {
        case LogLevel::DEBUG:
            logLevelString = "NOTICE";
            break;
        case LogLevel::ERROR:
            logLevelString = "ERROR";
            break;
    }

    if(logLevel >= LOG_LEVEL) {
        std::printf("[%s] %s %s\n", logLevelString.c_str(), msg.c_str(), origin.c_str());
    }
}

void Log(const glm::vec3& v, const std::string& name) {
    std::cout 
        << name 
        << " " 
        << v.x 
        << " " 
        << v.y 
        << " " 
        << v.z 
        << " " 
        << std::endl;
}

void Log(const float& val, const std::string& name) {
    std::cout 
        << name 
        << " " 
        << val 
        << std::endl;
}

void Log(const char* val, const std::string& name) {
    std::cout 
        << name 
        << " " 
        << val 
        << std::endl;
}
