#pragma once
#include <string>

class ICrashReportDetail{
public:
    virtual ~ICrashReportDetail() = default;
    virtual std::string call() = 0;
};
