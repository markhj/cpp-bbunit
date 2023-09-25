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
#include <functional>
#include <algorithm>
#include <memory>
#include <regex>
#include <format>
#include <cstring>

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
        /**
         * Set the testing mode.
         * For most use-cases this method doesn't need to be called, but for
         * testing of the TestCase class itself it is required.
         *
         * @param toMode
         */
        void setMode(TestMode toMode)
        {
            mode = toMode;
        }

        /**
         * Assert that two integers are equal.
         * Generate a standardized message.
         *
         * @param expected
         * @param actual
         * @return TestResult
         */
        TestResult assertEquals(int expected, int actual)
        {
            return assertEquals(expected,
                                actual,
                                std::format("Assert that int {} matches {}", expected, actual).c_str());
        }

        /**
         * Assert that two integer values are identical
         *
         * @param expected
         * @param actual
         * @param message
         * @return TestResult
         */
        TestResult assertEquals(int expected, int actual, const char *message)
        {
            return assert(expected == actual, message);
        }

        /**
         * Assert that two doubles (or floats) are equal.
         * Generate a standardized message.
         *
         * @param expected
         * @param actual
         * @return TestResult
         */
        TestResult assertEquals(double expected, double actual)
        {
            return assertEquals(expected,
                          actual,
                          std::format("Assert that float/double {} matches {}", expected, actual).c_str());
        }

        /**
         * Assert that two double (or floats) are equal.
         *
         * @param expected
         * @param actual
         * @param message
         * @return TestResult
         */
        TestResult assertEquals(double expected, double actual, const char *message)
        {
            return assert(expected == actual, message);
        }

        /**
         * Assert that two boolean values are identical.
         * Generate standardized message.
         *
         * @param expected
         * @param actual
         * @return TestResult
         */
        TestResult assertEquals(bool expected, bool actual)
        {
            return assertEquals(expected,
                                actual,
                          std::format("Assert that bool {} matches {}",
                                      expected,
                                      actual).c_str());
        }

        /**
         * Assert that two boolean values are identical.
         *
         * @param expected
         * @param actual
         * @param message
         * @returnTestResult
         */
        TestResult assertEquals(bool expected, bool actual, const char *message)
        {
            return assert(expected == actual, message);
        }

        /**
         * Assert that two const char match.
         * Generates standardized message.
         *
         * @param expected
         * @param actual
         * @return TestResult
         */
        TestResult assertEquals(const char *expected, const char *actual)
        {
            return assertEquals(expected,
                                actual,
                                std::format(R"(Assert that "{}" matches "{}")",
                                            expected,
                                            actual).c_str());
        }

        /**
         * Assert that two const char are identical.
         *
         * @param expected
         * @param actual
         * @param message
         * @return TestResult
         */
        TestResult assertEquals(const char *expected, const char *actual, const char *message)
        {
            std::string a = expected,
                b = actual;
            return assert(a == b, message);
        }

        /**
         * Assert that two std::string match.
         * Generates standard message.
         *
         * @param expected
         * @param actual
         * @return TestResult
         */
        TestResult assertEquals(const std::string& expected, const std::string& actual)
        {
            return assertEquals(expected,
                                actual,
                                std::format(R"(Assert that "{}" matches "{}")",
                                            expected,
                                            actual).c_str());
        }

        /**
         * Assert that two std::string match.
         *
         * @param expected
         * @param actual
         * @param message
         * @return TestResult
         */
        TestResult assertEquals(const std::string& expected, const std::string& actual, const char *message)
        {
            return assert(expected == actual, message);
        }

        /**
         * Assert than an integer is greater than another.
         * Generates standard message.
         *
         * @param compare
         * @param actual
         * @return TestResult
         */
        TestResult assertGreaterThan(int compare, int actual)
        {
            return assertGreaterThan(compare,
                                     actual,
                                     std::format(R"(Assert that int {} is greater than {})",
                                                 actual,
                                                 compare).c_str());
        }

        /**
         * Assert than an integer is greater than another.
         *
         * @param compare
         * @param actual
         * @param message
         * @return TestResult
         */
        TestResult assertGreaterThan(int compare, int actual, const char *message)
        {
            return assert(actual > compare, message);
        }

        /**
         * Assert that double (or float) is greater than another
         * Generates standard message.
         *
         * @param compare
         * @param actual
         * @return TestResult
         */
        TestResult assertGreaterThan(double compare, double actual)
        {
            return assertGreaterThan(compare,
                                     actual,
                                     std::format(R"(Assert that double/float {} is greater than {})",
                                                 actual,
                                                 compare).c_str());
        }

        /**
         * Assert that double (or float) is greater than another.
         *
         * @param compare
         * @param actual
         * @param message
         * @return TestResult
         */
        TestResult assertGreaterThan(double compare, double actual, const char *message)
        {
            return assert(actual > compare, message);
        }

        /**
         * Assert that an integer is greater than or equal to another.
         * Generates standard message.
         *
         * @param compare
         * @param actual
         * @return TestResult
         */
        TestResult assertGreaterThanOrEqual(int compare, int actual)
        {
            return assertGreaterThanOrEqual(compare,
                                            actual,
                                            std::format(R"(Assert that int {} is greater than or equal to {})",
                                                        actual,
                                                        compare).c_str());
        }

        /**
         * Assert than an integer is greater than or equal to another.
         *
         * @param compare
         * @param actual
         * @param message
         * @return TestResult
         */
        TestResult assertGreaterThanOrEqual(int compare, int actual, const char *message)
        {
            return assert(actual >= compare, message);
        }

        /**
         * Assert that a double (or float) is greater than or equal to another.
         *
         * @param compare
         * @param actual
         * @return TestResult
         */
        TestResult assertGreaterThanOrEqual(double compare, double actual)
        {
            return assertGreaterThanOrEqual(compare,
                          actual,
                          std::format(R"(Assert that double/float {} is greater than or equal to {})",
                                      actual,
                                      compare).c_str());
        }

        /**
         * Assert that a float/double is greater than or equal to another.
         *
         * @param compare
         * @param actual
         * @param message
         * @return TestResult
         */
        TestResult assertGreaterThanOrEqual(double compare, double actual, const char *message)
        {
            return assert(actual >= compare, message);
        }

        /**
         * Assert that an integer is less than another.
         * Generates standard message.
         *
         * @param compare
         * @param actual
         * @return TestResult
         */
        TestResult assertLessThan(int compare, int actual)
        {
            return assertLessThan(compare,
                                  actual,
                                  std::format(R"(Assert that int {} is less than {})",
                                              actual,
                                              compare).c_str());
        }

        /**
         * Assert that an integer is less than another.
         *
         * @param compare
         * @param actual
         * @param message
         * @return TestResult
         */
        TestResult assertLessThan(int compare, int actual, const char *message)
        {
            return assert(actual < compare, message);
        }

        /**
         * Assert that a double (or float) is less than another.
         * Generates standard message.
         *
         * @param compare
         * @param actual
         * @return TestResult
         */
        TestResult assertLessThan(double compare, double actual)
        {
            return assertLessThan(compare,
                                  actual,
                                  std::format(R"(Assert that double/float {} is less than {})",
                                              actual,
                                              compare).c_str());
        }

        /**
         * Assert that a double (or float) is less than another.
         *
         * @param compare
         * @param actual
         * @param message
         * @return TestResult
         */
        TestResult assertLessThan(double compare, double actual, const char *message)
        {
            return assert(actual < compare, message);
        }

        /**
         * Assert than an integer is less than or equal to another.
         * Generates standard message.
         *
         * @param compare
         * @param actual
         * @return TestResult
         */
        TestResult assertLessThanOrEqual(int compare, int actual)
        {
            return assertLessThanOrEqual(compare,
                                  actual,
                                  std::format(R"(Assert that int {} is less than or equal {})",
                                              actual,
                                              compare).c_str());
        }

        /**
         * Assert that an integer is less than or equal to another.
         *
         * @param compare
         * @param actual
         * @param message
         * @return TestResult
         */
        TestResult assertLessThanOrEqual(int compare, int actual, const char *message)
        {
            return assert(actual <= compare, message);
        }

        /**
         * Assert that a double (or float) is less than or equal to another.
         * Generates standard message.
         *
         * @param compare
         * @param actual
         * @return TestResult
         */
        TestResult assertLessThanOrEqual(double compare, double actual)
        {
            return assertLessThanOrEqual(compare, actual,
             std::format(R"(Assert that double/float {} is less than or equal {})",
                         actual,
                         compare).c_str());
        }

        /**
         * Assert that a double (or float) is less than or equal to another.
         *
         * @param compare
         * @param actual
         * @param message
         * @return TestResult
         */
        TestResult assertLessThanOrEqual(double compare, double actual, const char *message)
        {
            return assert(actual <= compare, message);
        }

        /**
         * Assert that a boolean value is true.
         *
         * @param actual
         * @param message
         * @return TestResult
         */
        TestResult assertTrue(bool actual, const char* message)
        {
            return assert(actual, message);
        }

        /**
         * Assert that a boolean is true.
         * Generate a standardized message.
         * 
         * @param actual
         * @return TestResult
         */
        TestResult assertTrue(bool actual)
        {
            return assertTrue(actual, "Assert that bool is true");
        }

        /**
         * Assert that a boolean value is false.
         *
         * @param actual
         * @param message
         * @return TestResult
         */
        TestResult assertFalse(bool actual, const char* message)
        {
            return assert(!actual, message);
        }

        /**
         * Assert that a boolean is false.
         * Generate a standardized message.
         *
         * @param actual
         * @return TestResult
         */
        TestResult assertFalse(bool actual)
        {
            return assertFalse(actual, "Assert that bool is false");
        }

        /**
         * Assert that a vector has a certain number of items.
         * Generates standard message.
         *
         * @tparam T
         * @param expected
         * @param vector
         * @return TestResult
         */
        template <typename T>
        TestResult assertCount(unsigned int expected, std::vector<T> vector)
        {
            return assertCount(expected,
                               vector,
                               std::format(R"(Assert that vector contains {} items)", expected).c_str());
        }

        /**
         * Assert that vector contains a certain number of items.
         *
         * @tparam T
         * @param expected
         * @param vector
         * @param message
         * @return TestResult
         */
        template <typename T>
        TestResult assertCount(unsigned int expected, std::vector<T> vector, const char* message)
        {
            return assert(expected == vector.size(), message);
        }

        /**
         * Assert that map contains a certain number of items.
         * Generates standard message.
         *
         * @tparam K
         * @tparam V
         * @param expected
         * @param map
         * @return TestResult
         */
        template <typename K, typename V>
        TestResult assertCount(unsigned int expected, std::map<K, V> map)
        {
            return assertCount(expected,
                               map,
                               std::format(R"(Assert that map contains {} items)", expected).c_str());
        }

        /**
         * Assert that map contains a specified number of items.
         *
         * @tparam K
         * @tparam V
         * @param expected
         * @param map
         * @param message
         * @return TestResult
         */
        template <typename K, typename V>
        TestResult assertCount(unsigned int expected, std::map<K, V> map, const char* message)
        {
            return assert(expected == map.size(), message);
        }

        /**
         * Assert that vector is empty.
         * Generates standard message.
         *
         * @tparam T
         * @param vector
         * @return TestResult
         */
        template <typename T>
        TestResult assertEmpty(std::vector<T> vector)
        {
            return assertEmpty(vector,
                               std::format(R"(Assert that vector is empty)").c_str());
        }

        /**
         * Assert that vector is empty.
         *
         * @tparam T
         * @param vector
         * @param message
         * @return TestResult
         */
        template <typename T>
        TestResult assertEmpty(std::vector<T> vector, const char* message)
        {
            return assert(vector.size() == 0, message);
        }

        /**
         * Assert that map is empty.
         * Generates standard message.
         *
         * @tparam K
         * @tparam V
         * @param map
         * @return TestResult
         */
        template <typename K, typename V>
        TestResult assertEmpty(std::map<K, V> map)
        {
            return assertEmpty(map,
                               std::format(R"(Assert that map is empty)").c_str());
        }

        /**
         * Assert that map is empty.
         *
         * @tparam K
         * @tparam V
         * @param map
         * @param message
         * @return TestResult
         */
        template <typename K, typename V>
        TestResult assertEmpty(std::map<K, V> map, const char* message)
        {
            return assert(map.size() == 0, message);
        }

        /**
         * Assert a string contains another string.
         * Generates standard message.
         *
         * @param contains
         * @param actual
         * @return TestResult
         */
        TestResult assertContains(const std::string& contains, const std::string& actual)
        {
            return assertContains(contains,
                                  actual,
                                  std::format(R"(String contains "{}")",
                                              shorten(contains)).c_str());
        }

        /**
         * Assert that a string contains another string.
         *
         * @param contains
         * @param actual
         * @param message
         * @return TestResult
         */
        TestResult assertContains(const std::string& contains, const std::string& actual, const char* message)
        {
            return assert(actual.find(contains) != std::string::npos, message);
        }

        /**
         * Assert that a string contains another string.
         * Case-insensitive version.
         * Generates standard message.
         *
         * @param contains
         * @param actual
         * @return TestResult
         */
        TestResult assertContainsCI(const std::string& contains, const std::string& actual)
        {
            return assertContainsCI(contains,
                                  actual,
                                  std::format(R"(String contains (case-insensitive) "{}")",
                                              shorten(contains)).c_str());
        }

        /**
         * Assert that a string contains another string.
         * Case-insensitive version.
         *
         * @param contains
         * @param actual
         * @param message
         * @return TestResult
         */
        TestResult assertContainsCI(const std::string& contains, const std::string& actual, const char* message)
        {
            std::string lwContains = contains, lwActual = actual;

            std::transform(lwContains.begin(), lwContains.end(), lwContains.begin(), ::tolower);
            std::transform(lwActual.begin(), lwActual.end(), lwActual.begin(), ::tolower);

            return assert(lwActual.find(lwContains) != std::string::npos, message);
        }

        /**
         * Assert (any type of) exception is thrown.
         * Generates standard message.
         *
         * @param func
         * @return TestResult
         */
        TestResult assertException(std::function<void()> func)
        {
            return assertException(func, "Assert that (any) exception is thrown");
        }

        /**
         * Assert (any type of) exception is thrown.
         *
         * @param func
         * @param message
         * @return TestResult
         */
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

        /**
         * Assert an exception of a specific type is thrown.
         * Generates standard message.
         *
         * @tparam ExceptionType
         * @param func
         * @return TestResult
         */
        template <typename ExceptionType>
        TestResult assertException(std::function<void()> func)
        {
            return assertException<ExceptionType>(func,
                  std::format(R"(Assert exception of type {} is thrown)", getClassName<ExceptionType>()).c_str());
        }

        /**
         * Assert that exception of specific type is thrown.
         *
         * @tparam ExceptionType
         * @param func
         * @param message
         * @return TestResult
         */
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

        /**
         * Assert exception message.
         * Generates standard message.
         *
         * @param expected
         * @param func
         * @return TestResult
         */
        TestResult assertExceptionMessage(const std::string& expected,
                                          const std::function<void()>& func)
        {
            return assertExceptionMessage(expected,
                                   func,
                                   std::format(R"(Assert exception message is: {})",
                                               shorten(expected)).c_str());
        }

        /**
         * Assert exception message.
         *
         * @param expected
         * @param func
         * @param message
         * @return TestResult
         */
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

        /**
         * Assert exception message and exception class type.
         * Generates standard message.
         *
         * @tparam ExceptionType
         * @param expected
         * @param func
         * @return TestResult
         */
        template <typename ExceptionType>
        TestResult assertExceptionMessage(const std::string& expected,
                                          const std::function<void()>& func)
        {
            return assertExceptionMessage<ExceptionType>(expected,
                                   func,
                                   std::format(R"(Assert exception is of type {} and message is: {})",
                                               getClassName<ExceptionType>(),
                                               shorten(expected)).c_str());
        }

        /**
         * Assert exception message and type of exception class
         *
         * @tparam ExceptionType
         * @param expected
         * @param func
         * @param message
         * @return TestResult
         */
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

        /**
         * Assert that exception messages contains a specified string.
         * Generates standard message.
         *
         * @param contains
         * @param func
         * @return TestResult
         */
        TestResult assertExceptionMessageContains(const std::string& contains,
                                                  const std::function<void()>& func)
        {
            return assertExceptionMessageContains(contains,
                                                  func,
                                                  std::format(R"(Assert exception message contains: {})",
                                                  shorten(contains)).c_str());
        }

        /**
         * Assert that exception message contains a specified string.
         *
         * @param contains
         * @param func
         * @param message
         * @return TestResult
         */
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

        /**
         * Case-insensitive version of assertExceptionMessageContains.
         * Generates standard message.
         *
         * @param contains
         * @param func
         * @return TestResult
         */
        TestResult assertExceptionMessageContainsCI(const std::string& contains,
                                                  const std::function<void()>& func)
        {
            return assertExceptionMessageContainsCI(contains,
              func,
              std::format(R"(Assert exception message contains (case-insensitive): {})",
                          shorten(contains)).c_str());
        }

        /**
         * Case-insensitive version of assertExceptionMessageContains.
         *
         * @param contains
         * @param func
         * @param message
         * @return TestResult
         */
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

        /**
         * Asert that exception message contains a specified string and is of a
         * specified type.
         *
         * Generates standard message.
         *
         * @tparam ExceptionType
         * @param contains
         * @param func
         * @return TestResult
         */
        template <typename ExceptionType>
        TestResult assertExceptionMessageContains(const std::string& contains,
                                                  const std::function<void()>& func)
        {
            return assertExceptionMessageContains<ExceptionType>(contains,
                func,
                std::format(R"(Assert exception is of type {} and message contains: {})",
                getClassName<ExceptionType>(),
                shorten(contains)).c_str());
        }

        /**
         * Asert that exception message contains a specified string and is of a
         * specified type.
         *
         * @tparam ExceptionType
         * @param contains
         * @param func
         * @return TestResult
         */
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

        /**
         * Case-insensitive version of assertExceptionMessageContains.
         *
         * Generates standard message.
         *
         * @tparam ExceptionType
         * @param contains
         * @param func
         * @return TestResult
         */
        template <typename ExceptionType>
        TestResult assertExceptionMessageContainsCI(const std::string& contains,
                                                  const std::function<void()>& func)
        {
            return assertExceptionMessageContainsCI<ExceptionType>(contains,
                  func,
                  std::format(R"(Assert exception is of type {} and message contains (case-insensitive): {})",
                              getClassName<ExceptionType>(),
                              shorten(contains)).c_str());
        }

        /**
         * Case-insensitive version of assertExceptionMessageContains (with template argument).
         *
         * Generates standard message.
         *
         * @tparam ExceptionType
         * @param contains
         * @param func
         * @return TestResult
         */
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

        /**
         * Assert that an exception message satisfies a regular expression.
         *
         * Generates standard message.
         *
         * @param regex
         * @param func
         * @return TestResult
         */
        TestResult assertExceptionMessageRegex(const char *regex,
                                               const std::function<void()>& func)
        {
            return assertExceptionMessageRegex(regex,
                                               func,
                                               std::format(R"(Assert exception satisfies regex: {})",
                                                           regex).c_str());
        }

        /**
         * Assert that an exception message satisfies a regular expression.
         *
         * @param regex
         * @param func
         * @return TestResult
         */
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

        /**
         * Assert that exception message satisfies regular expression, and
         * that the exception is of a specified type.
         *
         * Generates standard message.
         *
         * @tparam ExceptionType
         * @param regex
         * @param func
         * @return TestResult
         */
        template <typename ExceptionType>
        TestResult assertExceptionMessageRegex(const char *regex,
                                               const std::function<void()>& func)
        {
            return assertExceptionMessageRegex<ExceptionType>(regex,
                               func,
                               std::format(R"(Assert exception is of type {} and satisfies regex: {})",
                                           getClassName<ExceptionType>(),
                                           regex).c_str());
        }

        /**
         * Assert that exception message satisfies regular expression, and
         * that the exception is of a specified type.
         *
         * @tparam ExceptionType
         * @param regex
         * @param func
         * @return TestResult
         */
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

        /**
         * Assert that string satisfies regular expression.
         * Generates standard message.
         *
         * @param regex
         * @param actual
         * @return TestResult
         */
        TestResult assertRegex(const char* regex, std::string actual)
        {
            return assertRegex(regex,
                               actual,
                               std::format(R"(Assert that string satisfies regex: {})", regex).c_str());
        }

        /**
         * Assert that string satisfies regular expression.
         *
         * @param regex
         * @param actual
         * @param message
         * @return TestResult
         */
        TestResult assertRegex(const char* regex, std::string actual, const char* message)
        {
            return assert(std::regex_match(actual, std::regex(regex)), message);
        }

        /**
         * Asserts that a TestResult is true.
         *
         * Mainly used for self-testing this library.
         *
         * @param testResult
         */
        void assertTrue(const TestResult& testResult)
        {
            selfTesting([&]() {
                assert(testResult.passed, testResult.message);
            });
        }

        /**
         * Asserts that a TestResult is false.
         *
         * Mainly used for self-testing this library.
         *
         * @param testResult
         */
        void assertFalse(const TestResult& testResult)
        {
            selfTesting([&]() {
                assert(!testResult.passed, testResult.message);
            });
        }

        /**
         * Helper function for carrying out self-testing, in essence
         * managing that tested assertions are not printed.
         *
         * @param func
         */
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

        std::string shorten(std::string input) const
        {
            int maxLen = 20;
            if (input.length() <= maxLen)
                return input;

            return input.substr(0, maxLen - 4) + "...";
        }

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

        template <typename ClassName>
        std::string getClassName()
        {
            return typeid(ClassName).name();
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
