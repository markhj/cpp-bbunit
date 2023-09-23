#ifndef TESTLIB_TEST_BBUNIT_HPP
#define TESTLIB_TEST_BBUNIT_HPP

#ifdef _WIN32
#include <windows.h>
#endif

#include <iostream>
#include <utility>
#include <vector>
#include <map>
#include <array>
#include <tuple>
#include <functional>
#include <algorithm>
#include <memory>
#include <regex>

#ifdef _WIN32
const int FOREGROUND_RESET = 7;
const int BACKGROUND_RESET = 0;

void setColor(int colorCode)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorCode);
}
#else
const int FOREGROUND_GREEN = 0;
const int BACKGROUND_RED = 0;

void setColor(int colorCode)
{
    // @todo To be implemented
}
#endif

namespace BBUnit {

    enum TestMode {
        Standard = 1,
        SelfTest = 2,
    };

    enum PrintMode {
        FocusOnFail = 10,
        FullList = 20,
    };

    struct TestResult {
        bool passed;
        std::string message;
    };

    struct TestSummary {
        unsigned int assertions = 0, passed = 0;
    };

    class MustBeSelfTestingException : public std::exception {
    public:
        [[nodiscard]] const char* what() const noexcept override {
            return "Test case must be in self-testing mode to use this function.";
        }
    };

    class TestCase {
    public:
        [[nodiscard]] unsigned int getAssertions() const
        {
            return assertions;
        }

        [[nodiscard]] unsigned int getPassed() const
        {
            return passed;
        }

        TestCase& setPrintMode(PrintMode newPrintMode)
        {
            printMode = newPrintMode;
            return *this;
        }

    protected:
        void setMode(TestMode toMode)
        {
            mode = toMode;
        }

        TestResult assertEquals(int expected, int actual, const char *message)
        {
            return assert(expected == actual, message);
        }

        TestResult assertEquals(double expected, double actual, const char *message)
        {
            return assert(expected == actual, message);
        }

        TestResult assertEquals(bool expected, bool actual, const char *message)
        {
            return assert(expected == actual, message);
        }

        TestResult assertEquals(const char *expected, const char *actual, const char *message)
        {
            std::string a = expected,
                b = actual;
            return assert(a == b, message);
        }

        TestResult assertEquals(const std::string& expected, const std::string& actual, const char *message)
        {
            return assert(expected == actual, message);
        }

        TestResult assertGreaterThan(int compare, int actual, const char *message)
        {
            return assert(actual > compare, message);
        }

        TestResult assertGreaterThan(double compare, double actual, const char *message)
        {
            return assert(actual > compare, message);
        }

        TestResult assertGreaterThanOrEqual(int compare, int actual, const char *message)
        {
            return assert(actual >= compare, message);
        }

        TestResult assertGreaterThanOrEqual(double compare, double actual, const char *message)
        {
            return assert(actual >= compare, message);
        }

        TestResult assertLessThan(int compare, int actual, const char *message)
        {
            return assert(actual < compare, message);
        }

        TestResult assertLessThan(double compare, double actual, const char *message)
        {
            return assert(actual < compare, message);
        }

        TestResult assertLessThanOrEqual(int compare, int actual, const char *message)
        {
            return assert(actual <= compare, message);
        }

        TestResult assertLessThanOrEqual(double compare, double actual, const char *message)
        {
            return assert(actual <= compare, message);
        }

        TestResult assertTrue(bool actual, const char* message)
        {
            return assert(actual, message);
        }

        TestResult assertFalse(bool actual, const char* message)
        {
            return assert(!actual, message);
        }

        template <typename T>
        TestResult assertCount(unsigned int expected, std::vector<T> vector, const char* message)
        {
            return assert(expected == vector.size(), message);
        }

        template <typename T>
        TestResult assertEmpty(std::vector<T> vector, const char* message)
        {
            return assert(vector.size() == 0, message);
        }

        template <typename K, typename V>
        TestResult assertCount(unsigned int expected, std::map<K, V> map, const char* message)
        {
            return assert(expected == map.size(), message);
        }

        template <typename K, typename V>
        TestResult assertEmpty(std::map<K, V> map, const char* message)
        {
            return assert(map.size() == 0, message);
        }

        TestResult assertContains(const char *contains, const std::string& actual, const char* message)
        {
            return assert(actual.find(contains) != std::string::npos, message);
        }

        TestResult assertContainsCI(const char *contains, const std::string& actual, const char* message)
        {
            std::string lwContains = contains, lwActual = actual;

            std::transform(lwContains.begin(), lwContains.end(), lwContains.begin(), ::tolower);
            std::transform(lwActual.begin(), lwActual.end(), lwActual.begin(), ::tolower);

            return assert(lwActual.find(lwContains) != std::string::npos, message);
        }

        TestResult assertException(std::function<void()> func, const char* message)
        {
            bool caught = false;
            try {
                func();
            } catch (...) {
                caught = true;
            }

            return assert(caught, message);
        }

        TestResult assertExceptionMessage(const char* expected,
            const std::function<void()>& func,
            const char* message)
        {
            return assertExceptionMessage(std::string(expected), func, message);
        }

        TestResult assertExceptionMessage(const std::string& expected,
            const std::function<void()>& func,
            const char* message)
        {
            std::string msg;
            try {
                func();
            } catch (std::exception& e) {
                msg = e.what();
            }

            return assert(msg == expected, message);
        }

        template <typename ExceptionType>
        TestResult assertExceptionMessage(const char* expected,
            const std::function<void()>& func,
            const char* message)
        {
            return assertExceptionMessage<ExceptionType>(std::string(expected), func, message);
        }

        template <typename ExceptionType>
        TestResult assertExceptionMessage(const std::string& expected,
            const std::function<void()>& func,
            const char* message)
        {
            std::string msg;
            try {
                func();
            } catch (ExceptionType& e) {
                return assert(e.what() == expected, message);
            } catch (...) {
                // Do nothing
            }

            return assert(false, message);
        }

        TestResult assertExceptionMessageContains(const char* contains,
              const std::function<void()>& func,
              const char* message)
        {
            return assertExceptionMessageContains(std::string(contains), func, message);
        }

        TestResult assertExceptionMessageContains(const std::string& contains,
              const std::function<void()>& func,
              const char* message)
        {
            std::string msg;
            try {
                func();
            } catch (std::exception& e) {
                msg = e.what();
            }

            return assert(msg.find(contains) != std::string::npos, message);
        }

        TestResult assertExceptionMessageRegex(const char *regex,
            const std::function<void()>& func,
            const char* message)
        {
            std::string msg;
            try {
                func();
            } catch (std::exception& e) {
                msg = e.what();
            }

            if (msg.empty()) {
                return assert(false, message);
            }

            return assertRegex(regex, msg, message);
        }

        template <typename ExceptionType>
        TestResult assertExceptionMessageRegex(const char *regex,
            const std::function<void()>& func,
            const char* message)
        {
            try {
                func();
            } catch (ExceptionType& e) {
                return assertRegex(regex, e.what(), message);
            } catch (...) {
                // Do nothing
            }

            return assert(false, message);
        }

        TestResult assertExceptionMessageContainsCI(const char* contains,
            const std::function<void()>& func,
            const char* message)
        {
            return assertExceptionMessageContainsCI(std::string(contains), func, message);
        }

        TestResult assertExceptionMessageContainsCI(const std::string& contains,
            const std::function<void()>& func,
            const char* message)
        {
            std::string msg;
            try {
                func();
            } catch (std::exception& e) {
                std::string lwContains = contains, lwActual = e.what();

                std::transform(lwContains.begin(), lwContains.end(), lwContains.begin(), ::tolower);
                std::transform(lwActual.begin(), lwActual.end(), lwActual.begin(), ::tolower);

                return assert(lwActual.find(lwContains) != std::string::npos, message);
            }

            return assert(false, message);
        }

        template <typename ExceptionType>
        TestResult assertExceptionMessageContains(const char* contains,
            const std::function<void()>& func,
            const char* message)
        {
            return assertExceptionMessageContains<ExceptionType>(std::string(contains), func, message);
        }

        template <typename ExceptionType>
        TestResult assertExceptionMessageContains(const std::string& contains,
            const std::function<void()>& func,
            const char* message)
        {
            bool caught = false;
            try {
                func();
            } catch (ExceptionType& e) {
                return assert(std::string(e.what()).find(contains) != std::string::npos, message);
            } catch (...) {
                // Do nothing
            }

            return assert(false, message);
        }

        template <typename ExceptionType>
        TestResult assertExceptionMessageContainsCI(const char* contains,
            const std::function<void()>& func,
            const char* message)
        {
            return assertExceptionMessageContainsCI<ExceptionType>(std::string(contains), func, message);
        }

        template <typename ExceptionType>
        TestResult assertExceptionMessageContainsCI(const std::string& contains,
            const std::function<void()>& func,
            const char* message)
        {
            bool caught = false;
            try {
                func();
            } catch (ExceptionType& e) {
                std::string lwContains = contains, lwActual = e.what();

                std::transform(lwContains.begin(), lwContains.end(), lwContains.begin(), ::tolower);
                std::transform(lwActual.begin(), lwActual.end(), lwActual.begin(), ::tolower);

                return assert(lwActual.find(lwContains) != std::string::npos, message);
            } catch (...) {
                // Do nothing
            }

            return assert(false, message);
        }

        template <typename ExceptionType>
        TestResult assertException(std::function<void()> func, const char* message)
        {
            bool caught = false;
            try {
                func();
            } catch (ExceptionType& e) {
                caught = true;
            } catch (...) {
                // Do nothing
            }

            return assert(caught, message);
        }

        TestResult assertRegex(const char* regex, std::string actual, const char* message)
        {
            return assert(std::regex_match(actual, std::regex(regex)), message);
        }

        void assertTrue(const TestResult& testResult)
        {
            selfTesting([&]() {
                assert(testResult.passed, testResult.message);
            });
        }

        void assertFalse(const TestResult& testResult)
        {
            selfTesting([&]() {
                assert(!testResult.passed, testResult.message);
            });
        }

        void selfTesting(const std::function<void()>& func)
        {
            if (mode != TestMode::SelfTest) {
                throw MustBeSelfTestingException();
            }

            selfTestingScope = true;

            func();

            selfTestingScope = false;
        }

    private:
        TestMode mode = TestMode::Standard;

        PrintMode printMode = PrintMode::FocusOnFail;

        bool selfTestingScope = false;

        unsigned int assertions = 0,
            passed = 0;

        void printResult(bool testPassed, const std::string& message)
        {
            setColor(FOREGROUND_RESET | BACKGROUND_RESET);

            switch (printMode) {
                case PrintMode::FocusOnFail:
                    printFocusOnFail(testPassed, message);
                    break;
                case PrintMode::FullList:
                    printFullList(testPassed, message);
                    break;
            }
        }

        void printFocusOnFail(bool testPassed, const std::string& message)
        {
            if (testPassed) {
                return;
            }

            setColor(BACKGROUND_RED);
            std::cout << "\n FAILED ";
            setColor(FOREGROUND_RESET | BACKGROUND_RESET);
            std::cout << " " << message << std::endl;
        }

        void printFullList(bool testPassed, const std::string& message)
        {
            if (testPassed) {
                setColor(FOREGROUND_GREEN);
                std::cout << "  OK  ";
            } else {
                setColor(BACKGROUND_RED);
                std::cout << " FAIL ";
            }

            setColor(FOREGROUND_RESET | BACKGROUND_RESET);
            std::cout << " " << message << std::endl;
        }

        TestResult assert(bool testPassed, const std::string& message)
        {
            if (mode == TestMode::Standard
                || (mode == TestMode::SelfTest && selfTestingScope)
                ) {
                assertions++;

                if (testPassed)
                    passed++;

                printResult(testPassed, message);
            }

            return {
                .passed = testPassed,
                .message = message,
            };
        }

    };

    template<typename TC>
    class TestSuite {
    public:
        template <typename... Funcs>
        explicit TestSuite(Funcs... funcs) : current(TC())
        {
            (functions.push_back([&, funcs](PrintMode pMode) {
                current.setPrintMode(pMode);
                (current.*funcs)();
                return TestSummary {
                    .assertions = current.getAssertions(),
                    .passed = current.getPassed()
                };
            }), ...);
        }

        TestSummary run()
        {
            TestSummary total;

            for (const auto& testFunc : functions) {
                TestSummary summary = testFunc(printMode);

                total.assertions = summary.assertions;
                total.passed = summary.passed;
            }

            return total;
        }

        TestSuite& setPrintMode(PrintMode newPrintMode)
        {
            printMode = newPrintMode;
            return *this;
        }

    private:
        TC current;

        std::vector<std::function<TestSummary(PrintMode pMode)>> functions;

        PrintMode printMode = PrintMode::FocusOnFail;
    };

    class TestRunner {
    public:
        template<typename... TestSuites>
        void run(TestSuites... suites)
        {
            std::cout << "C++ BBUnit" << std::endl;

            (runSuite(suites), ...);

            summarize();
        }

        void summarize() const
        {
            std::cout << "\n-------------------------------------------------\n";

            if (passed == assertions) {
                setColor(BACKGROUND_GREEN);
                std::cout << " NICE! ";
                setColor(BACKGROUND_RESET);
                std::cout << " ";
            } else {
                setColor(FOREGROUND_RED);
                std::cout << "Failed: " << (assertions - passed);
                setColor(FOREGROUND_RESET);
                std::cout << " | ";
            }

            setColor(BACKGROUND_RESET | FOREGROUND_RESET);
            std::cout << "Passed: " << passed << " | Total: " << assertions << std::endl;
        }

        TestRunner& setPrintMode(PrintMode newPrintMode)
        {
            printMode = newPrintMode;
            return *this;
        }

    private:
        unsigned int assertions = 0, passed = 0;

        PrintMode printMode = PrintMode::FocusOnFail;

        template <typename TCD>
        void runSuite(TestSuite<TCD> testSuite)
        {
            TestSummary summary = testSuite.setPrintMode(printMode).run();

            assertions += summary.assertions;
            passed += summary.passed;
        }

    };

}

#endif
