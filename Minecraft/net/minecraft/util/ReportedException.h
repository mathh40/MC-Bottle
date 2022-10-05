#pragma once
#include "../crash/CrashReport.h"

class CrashReport;

class ReportedException : public std::exception
{
  public:
    ReportedException(CrashReport &report);
    CrashReport &getCrashReport();
    std::exception &getCause() const;
    char const *what() const override;

  private:
    CrashReport crashReport;
};
