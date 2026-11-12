#pragma once
// Minimal self-contained unit test framework (no external dependency).
//
//   TEST_CASE("Suite", "name") { CHECK(a == b); REQUIRE(ptr != nullptr); }
//
// The runner (TestMain.cpp) accepts:
//   --list              print every "Suite.name"
//   --file <TestX>      run only the tests defined in TestX.cpp (used by CTest)
//   <filter>            run only tests whose "Suite.name" contains <filter>
#include <cmath>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace giera_test {

struct TestCase {
    std::string suite;
    std::string name;
    std::function<void()> body;
    std::string file; ///< source file stem, e.g. "TestCombat"
    std::string fullName() const { return suite + "." + name; }
};

inline std::vector<TestCase>& registry() {
    static std::vector<TestCase> tests;
    return tests;
}

struct Registrar {
    Registrar(const char* suite, const char* name, std::function<void()> body, const char* file) {
        std::string path(file);
        size_t slash = path.find_last_of("/\\");
        std::string stem = slash == std::string::npos ? path : path.substr(slash + 1);
        stem = stem.substr(0, stem.find('.'));
        registry().push_back({ suite, name, std::move(body), stem });
    }
};

struct RequireFailed {};

struct Context {
    int failedChecks = 0;
    int totalChecks = 0;
};

inline Context& context() {
    static Context ctx;
    return ctx;
}

inline void reportFailure(const char* file, int line, const std::string& what) {
    context().failedChecks++;
    std::cerr << "    " << file << "(" << line << "): FAILED: " << what << "\n";
}

inline bool approxEqual(double a, double b, double eps = 1e-9) {
    return std::fabs(a - b) <= eps * std::max(1.0, std::max(std::fabs(a), std::fabs(b)));
}

template <typename A, typename B>
std::string describeComparison(const A& a, const B& b) {
    std::ostringstream ss;
    ss << " (lhs: " << a << ", rhs: " << b << ")";
    return ss.str();
}

} // namespace giera_test

#define GT_CONCAT_INNER(a, b) a##b
#define GT_CONCAT(a, b) GT_CONCAT_INNER(a, b)

#define TEST_CASE(suite, name)                                                                     \
    static void GT_CONCAT(giera_test_fn_, __LINE__)();                                             \
    static giera_test::Registrar GT_CONCAT(giera_test_reg_, __LINE__)(                             \
        suite, name, &GT_CONCAT(giera_test_fn_, __LINE__), __FILE__);                                        \
    static void GT_CONCAT(giera_test_fn_, __LINE__)()

#define CHECK(expr)                                                                                \
    do {                                                                                           \
        giera_test::context().totalChecks++;                                                       \
        if (!(expr)) giera_test::reportFailure(__FILE__, __LINE__, #expr);                         \
    } while (0)

#define REQUIRE(expr)                                                                              \
    do {                                                                                           \
        giera_test::context().totalChecks++;                                                       \
        if (!(expr)) {                                                                             \
            giera_test::reportFailure(__FILE__, __LINE__, "REQUIRE " #expr);                       \
            throw giera_test::RequireFailed{};                                                     \
        }                                                                                          \
    } while (0)

#define CHECK_EQ(a, b)                                                                             \
    do {                                                                                           \
        giera_test::context().totalChecks++;                                                       \
        auto&& gt_a = (a);                                                                         \
        auto&& gt_b = (b);                                                                         \
        if (!(gt_a == gt_b))                                                                       \
            giera_test::reportFailure(__FILE__, __LINE__,                                          \
                #a " == " #b + giera_test::describeComparison(gt_a, gt_b));                        \
    } while (0)

#define CHECK_APPROX(a, b)                                                                         \
    do {                                                                                           \
        giera_test::context().totalChecks++;                                                       \
        double gt_a = (a);                                                                         \
        double gt_b = (b);                                                                         \
        if (!giera_test::approxEqual(gt_a, gt_b))                                                  \
            giera_test::reportFailure(__FILE__, __LINE__,                                          \
                #a " ~= " #b + giera_test::describeComparison(gt_a, gt_b));                        \
    } while (0)

#define CHECK_BETWEEN(value, lo, hi)                                                               \
    do {                                                                                           \
        giera_test::context().totalChecks++;                                                       \
        auto gt_v = (value);                                                                       \
        if (!(gt_v >= (lo) && gt_v <= (hi))) {                                                     \
            std::ostringstream gt_ss;                                                              \
            gt_ss << #value " in [" #lo ", " #hi "] (value: " << gt_v << ")";                      \
            giera_test::reportFailure(__FILE__, __LINE__, gt_ss.str());                            \
        }                                                                                          \
    } while (0)
