#include "CrashReport.h"

#include "ReportedException.h"
#include "Util.h"
#include "../../../../spdlog/include/spdlog/spdlog-inl.h"
#include "gen/layer/IntCache.h"

#include <fstream>
#include <Shlwapi.h>
#include <thread>

std::shared_ptr<spdlog::logger> CrashReport::LOGGER = spdlog::get("Minecraft")->clone("CrashReport");

CrashReport::CrashReport(std::string_view descriptionIn, std::exception &causeThrowable):
    description(descriptionIn), cause(causeThrowable),systemDetailsCategory(*this, "System Details") {
    populateEnvironment();
}

std::string_view CrashReport::getDescription() const {
    return description;
}

std::exception &CrashReport::getCrashCause() const {
    return cause;
}

void CrashReport::getSectionsInStringBuilder(std::string &builder) {
    if ((stacktrace == nullptr || stacktrace.size() <= 0) && !crashReportSections.empty()) {
        stacktrace = (StackTraceElement[])((StackTraceElement[])ArrayUtils.subarray(
            (crashReportSections[0]).getStackTrace(), 0, 1));
    }

    if (stacktrace != nullptr && stacktrace.size() > 0) {
        builder.append("-- Head --\n");
            builder.append("Thread: ").append(Thread.currentThread().getName()).append("\n");
        builder.append("Stacktrace:\n");

        for (auto stacktraceelement : stacktrace){
            builder.append("\t").append("at ").append(stacktraceelement);
            builder.append("\n");
        }

        builder.append("\n");
    }

    for (auto crashreportcategory : crashReportSections){
        crashreportcategory.appendToStringBuilder(builder);
        builder.append("\n\n");
    }

    systemDetailsCategory.appendToStringBuilder(builder);
}

std::string CrashReport::getCauseStackTraceOrString() {
    StringWriter stringwriter = null;
    PrintWriter printwriter = null;
    Throwable throwable = this.cause;
    if (((Throwable)throwable).getMessage() == null) {
        if (throwable instanceof NullPointerException) {
            throwable = new NullPointerException(description);
        } else if (throwable instanceof StackOverflowError) {
            throwable = new StackOverflowError(description);
        } else if (throwable instanceof OutOfMemoryError) {
            throwable = new OutOfMemoryError(description);
        }

        ((Throwable)throwable).setStackTrace(cause.getStackTrace());
    }

    String s = ((Throwable)throwable).toString();

    try {
        stringwriter = new StringWriter();
        printwriter = new PrintWriter(stringwriter);
        ((Throwable)throwable).printStackTrace(printwriter);
        s = stringwriter.toString();
    }
    finally{
            IOUtils.closeQuietly(stringwriter);
            IOUtils
            .closeQuietly(printwriter);
        }


    return s;
}

std::string CrashReport::getCompleteReport() const {
    std::string stringbuilder;
    stringbuilder.append("---- Minecraft Crash Report ----\n");
    stringbuilder.append("// ");
    stringbuilder.append(getWittyComment());
    stringbuilder.append("\n\n");
    stringbuilder.append("Time: ");
    stringbuilder.append((new SimpleDateFormat()).format(new Date()));
    stringbuilder.append("\n");
    stringbuilder.append("Description: ");
    stringbuilder.append(description);
    stringbuilder.append("\n\n");
    stringbuilder.append(getCauseStackTraceOrString());
    stringbuilder.append(
        "\n\nA detailed walkthrough of the error, its code path and all known details is as follows:\n");

    for (int i = 0; i < 87; ++i) {
        stringbuilder.append("-");
    }

    stringbuilder.append("\n\n");
    getSectionsInStringBuilder(stringbuilder);
    return stringbuilder;
}

std::string CrashReport::getFile() const {
    return crashReportFile.string();
}

bool CrashReport::saveToFile(std::filesystem::path &toFile) {
    if (!std::filesystem::exists(crashReportFile)) {
        return false;
    } else {
        if (!std::filesystem::exists(toFile.parent_path())) {
            std::filesystem::create_directory(toFile.parent_path());
        }

        try {
            std::fstream writer(toFile);
            auto temp = getCompleteReport();
            writer.write(temp.c_str(),temp.size());
            crashReportFile = toFile;
            return true;
        } catch (std::exception& var9) {
            LOGGER->error("Could not save crash report to {}", toFile, var9);
        }

        return false;
    }
}

CrashReportCategory &CrashReport::getCategory() {
    return systemDetailsCategory;
}

CrashReportCategory &CrashReport::makeCategory(std::string_view name) {
    return makeCategoryDepth(name, 1);
}

CrashReportCategory &CrashReport::makeCategoryDepth(std::string_view categoryName, int32_t stacktraceLength) {
    CrashReportCategory crashreportcategory = CrashReportCategory(*this, categoryName);
    if (firstCategoryInCrashReport) {
        auto i = crashreportcategory.getPrunedStackTrace(stacktraceLength);
        StackTraceElement [] astacktraceelement = cause.getStackTrace();
        StackTraceElement stacktraceelement = nullptr;
        StackTraceElement stacktraceelement1 = nullptr;
        auto j = astacktraceelement.size() - i;
        if (j < 0) {
            LOGGER->error("Negative index in crash report handler ({}/{})" , astacktraceelement.size(), i);
        }

        if (astacktraceelement != nullptr && 0 <= j && j < astacktraceelement.size()) {
            stacktraceelement = astacktraceelement[j];
            if (astacktraceelement.size() + 1 - i < astacktraceelement.size()) {
                stacktraceelement1 = astacktraceelement[astacktraceelement.size() + 1 - i];
            }
        }

        firstCategoryInCrashReport = crashreportcategory.firstTwoElementsOfStackTraceMatch(
            stacktraceelement, stacktraceelement1);
        if (i > 0 && !crashReportSections.empty()) {
            CrashReportCategory crashreportcategory1 = crashReportSections[
                crashReportSections.size() - 1];
            crashreportcategory1.trimStackTraceEntriesFromBottom(i);
        } else if (astacktraceelement != nullptr && astacktraceelement.size() >= i && 0 <= j && j < astacktraceelement.
                   size()) {
            stacktrace.resize(j);
            System.arraycopy(astacktraceelement, 0, stacktrace, 0, stacktrace.size());
        } else {
            firstCategoryInCrashReport = false;
        }
    }

    crashReportSections.emplace_back(crashreportcategory);
    return crashreportcategory;
}

CrashReport CrashReport::makeCrashReport(std::exception &causeIn, std::string_view descriptionIn) {
    if (Util::instanceof<ReportedException>(&causeIn)) {
        return ((ReportedException*)&causeIn)->getCrashReport();
    } else {
        return CrashReport(descriptionIn, causeIn);
    }
}

void CrashReport::populateEnvironment() {
    systemDetailsCategory.addDetail("Minecraft Version", [&]()
    {
            return "1.12.2";
    });

    systemDetailsCategory.addDetail("Operating System", [&]()
    {
            return System.getProperty("os.name") + " (" + System.getProperty("os.arch") + ") version " + System.
                   getProperty("os.version");
    });

    systemDetailsCategory.addDetail("Java Version", [&]()
    {
            return System.getProperty("java.version") + ", " + System.getProperty("java.vendor");
    });

    systemDetailsCategory.addDetail("Java VM Version", [&]()
    {
            return System.getProperty("java.vm.name") + " (" + System.getProperty("java.vm.info") + "), " + System.
    });

    systemDetailsCategory.addDetail("Memory", [&]()
    {
            Runtime runtime = Runtime.getRuntime();
            long i = runtime.maxMemory();
            long j = runtime.totalMemory();
            long k = runtime.freeMemory();
            long l = i / 1024L / 1024L;
            long i1 = j / 1024L / 1024L;
            long j1 = k / 1024L / 1024L;
            return k + " bytes (" + j1 + " MB) / " + j + " bytes (" + i1 + " MB) up to " + i + " bytes (" + l + " MB)";
    });

    systemDetailsCategory.addDetail("JVM Flags", [&]()
    {
            RuntimeMXBean runtimemxbean = ManagementFactory.getRuntimeMXBean();
            List list = runtimemxbean.getInputArguments();
            int i = 0;
            StringBuilder stringbuilder = new StringBuilder();
            Iterator var5 = list.iterator();

            while (var5.hasNext()) {
                String s = (String)var5.next();
                if (s.startsWith("-X")) {
                    if (i++ > 0) {
                        stringbuilder.append(" ");
                    }

                    stringbuilder.append(s);
                }
            }

            return fmt::format("{:d} total; {:s}", i, stringbuilder);
    });

    systemDetailsCategory.addDetail("IntCache",[&]()
    {
            return IntCache.getCacheSizes();
    });
}

std::string_view CrashReport::getWittyComment() {
    constexpr std::array<std::string_view,34>
    astring = {"Who set us up the TNT?",
                           "Everything's going to plan. No, really, that was supposed to happen.",
                           "Uh... Did I do that?", "Oops.", "Why did you do that?", "I feel sad now :(", "My bad.",
                           "I'm sorry, Dave.", "I let you down. Sorry :(",
                           "On the bright side, I bought you a teddy bear!", "Daisy, daisy...",
                           "Oh - I know what I did wrong!", "Hey, that tickles! Hehehe!", "I blame Dinnerbone.",
                           "You should try our sister game, Minceraft!",
                           "Don't be sad. I'll do better next time, I promise!", "Don't be sad, have a hug! <3",
                           "I just don't know what went wrong :(", "Shall we play a game?",
                           "Quite honestly, I wouldn't worry myself about that.",
                           "I bet Cylons wouldn't have this problem.", "Sorry :(",
                           "Surprise! Haha. Well, this is awkward.", "Would you like a cupcake?",
                           "Hi. I'm Minecraft, and I'm a crashaholic.", "Ooh. Shiny.", "This doesn't make any sense!",
                           "Why is it breaking :(", "Don't do that.", "Ouch. That hurt :(", "You're mean.",
                           "This is a token for 1 free hug. Redeem at your nearest Mojangsta: [~~HUG~~]",
                           "There are four lights!", "But it works on my machine."};

    try {
        return astring[(Util::getcurrent_time() % astring.size())];
    } catch (std::exception &var2) {
        return "Witty comment unavailable :(";
    }
}
