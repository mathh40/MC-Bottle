#pragma once
#include <stdexcept>


class ReportedException :public std::exception
{
public:
	ReportedException(CrashReport report);
	CrashReport getCrashReport();
	void getCause();
	char const* what() const override;
private:
	CrashReport crashReport;
};