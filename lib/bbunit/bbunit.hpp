#ifndef TESTLIB_TEST_BBUNIT_HPP
#define TESTLIB_TEST_BBUNIT_HPP

#include <iostream>
#include <vector>
#include <map>
#include <tuple>
#include <functional>

#ifdef _WIN32
#include <windows.h>
#endif

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
        Standard,
        SelfTest,
    };

    enum PrintMode {
        FocusOnFail,
        FullList,
    };

    struct TestResult {
        bool passed;
        std::string message;
    };

    class MustBeSelfTestingException : public std::exception {
    public:
        const char* what() const noexcept override {
            return "Test case must be in self-testing mode to use this function.";
        }
    };

    class TestCase {
    public:
        TestCase* run()
        {
            test();

            return this;
        }

        unsigned int getAssertions() const
        {
            return assertions;
        }

        unsigned int getPassed() const
        {
            return passed;
        }

    protected:
        virtual void test() = 0;

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

    class Dispatcher {
    public:
        template<typename... TestCases>
        static void dispatch(TestCases... testCases)
        {
            std::initializer_list<TestCase*> list = {testCases.run()...};

            unsigned int assertions = 0,
                passed = 0;

            for (auto item : list) {
                assertions += item->getAssertions();
                passed += item->getPassed();
            }

            summarize(assertions, passed);
        }

        static void summarize(int assertions, int passed)
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

    };

}

#endif
