#include "TestFramework.h"
#include "TestSupport.h"
#include <SDL.h>
#include <exception>
#if defined(_MSC_VER)
#include <crtdbg.h>
#include <cstdlib>
#endif

int main(int argc, char* argv[])
{
    using namespace giera_test;
#if defined(_MSC_VER)
    // Debug-CRT assertions (e.g. vector index out of range) must print and abort,
    // not open a modal dialog that makes the test run look like a hang.
    for (int type : { _CRT_WARN, _CRT_ERROR, _CRT_ASSERT }) {
        _CrtSetReportMode(type, _CRTDBG_MODE_FILE);
        _CrtSetReportFile(type, _CRTDBG_FILE_STDERR);
    }
    _set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);
#endif
    std::string filter;
    std::string fileFilter;
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--file" && i + 1 < argc) {
            fileFilter = argv[++i];
            continue;
        }
        if (arg == "--list") {
            for (auto& t : registry()) {
                std::cout << t.fullName() << "\n";
            }
            return 0;
        }
        filter = arg;
    }

    initTestEnvironment();

    int passed = 0, failed = 0, ran = 0;
    for (auto& t : registry()) {
        if (!filter.empty() && t.fullName().find(filter) == std::string::npos) {
            continue;
        }
        if (!fileFilter.empty() && t.file != fileFilter) {
            continue;
        }
        ran++;
        int failuresBefore = context().failedChecks;
        std::cout << "[ RUN  ] " << t.fullName() << "\n";
        try {
            t.body();
        }
        catch (const RequireFailed&) {
        }
        catch (const std::exception& e) {
            reportFailure(__FILE__, __LINE__, std::string("unexpected exception: ") + e.what());
        }
        catch (const char* e) {
            reportFailure(__FILE__, __LINE__, std::string("unexpected exception: ") + e);
        }
        catch (...) {
            reportFailure(__FILE__, __LINE__, "unexpected unknown exception");
        }
        if (context().failedChecks == failuresBefore) {
            passed++;
            std::cout << "[  OK  ] " << t.fullName() << "\n";
        }
        else {
            failed++;
            std::cout << "[ FAIL ] " << t.fullName() << "\n";
        }
    }

    shutdownTestEnvironment();

    if (ran == 0) {
        std::cerr << "No test matched filter '" << filter << "'\n";
        return 1;
    }
    std::cout << "\n" << passed << "/" << ran << " test cases passed, "
              << context().totalChecks << " checks, " << context().failedChecks << " failed\n";
    return failed == 0 ? 0 : 1;
}
