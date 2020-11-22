#pragma once
#include "CrashReportCategory.h"
#include "../../../../spdlog/include/spdlog/logger.h"

#include <filesystem>

class CrashReport {

public:
    CrashReport(std::string_view descriptionIn, std::exception &causeThrowable);
    std::string_view getDescription() const;
    std::exception &getCrashCause() const;
    void getSectionsInStringBuilder(std::string &builder);
    std::string getCauseStackTraceOrString();
    std::string getCompleteReport() const;
    std::string getFile() const;
    bool saveToFile(std::filesystem::path &toFile);
    CrashReportCategory &getCategory();
    CrashReportCategory &makeCategory(std::string_view name);
    CrashReportCategory &makeCategoryDepth(std::string_view categoryName, int32_t stacktraceLength);
    static CrashReport makeCrashReport(std::exception &causeIn, std::string_view descriptionIn);

private:
    void populateEnvironment();
    static std::string_view getWittyComment();

private:
    static std::shared_ptr<spdlog::logger> LOGGER;
    std::string description;
    std::exception& cause;
    CrashReportCategory systemDetailsCategory;
    std::vector<CrashReportCategory> crashReportSections;
    std::filesystem::path crashReportFile;
    bool firstCategoryInCrashReport = true;
    std::vector<StackTraceElement> stacktrace;
};
