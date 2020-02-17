#include "ReportedException.h"

ReportedException::ReportedException(CrashReport report)
	:crashReport(report)
{

}

CrashReport ReportedException::getCrashReport()
{
	return crashReport;
}

void ReportedException::getCause()
{
	return crashReport.getCrashCause();
}

char const * ReportedException::what() const
{
	return crashReport.getDescription();
}
