#include "ReportedException.h"

ReportedException::ReportedException(CrashReport& report)
	:crashReport(report)
{

}

CrashReport& ReportedException::getCrashReport()
{
	return crashReport;
}

std::exception& ReportedException::getCause() const {
	return crashReport.getCrashCause();
}

char const * ReportedException::what() const
{
	return crashReport.getDescription().data();
}
