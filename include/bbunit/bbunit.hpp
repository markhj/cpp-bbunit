/**
 * C++ BBUnit - Unit testing library
 *
 * This header-only library provides an easy and convenient way to
 * unit and feature test your C++ code.
 *
 * @see https://github.com/markhj/cpp-bbunit
 *      Repository on GitHub
 */

#pragma once

#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <regex>
#include <variant>
#include <vector>

namespace BBUnit {
    typedef uint16_t CaseNumber;

    /**
     * Concept used to ensure a type can be compared to itself.
     *
     * @tparam T
     */
    template<typename T>
    concept Comparable = requires(T a, T b) {
        { a == b } -> std::convertible_to<bool>;
    };

    /**
     * Concept which requires a ``size`` method which returns ``size_t``.
     * Used for ``assertCount``, and typically revolves around ``std::vector``,
     * ``std::map`` and ``std::array``.
     *
     * @tparam T
     */
    template<typename T>
    concept HasSizeMethod = requires(T t) {
        { t.size() } -> std::convertible_to<std::size_t>;
    };

    /**
     * Concept which requires a ``what`` method.
     * Mainly used to extract error messages from exceptions.
     *
     * @tparam T
     */
    template<typename T>
    concept HasWhatMethod = requires(T t) {
        { t.what() } -> std::convertible_to<std::string>;
    };

    /**
     * When an assertion cannot be performed, because an error
     * is caught.
     */
    enum class ErrorCode {
        /**
         * When the test settings enable ``stopAssertingAfterFail``, this error
         * code is used to indicate exactly that in following assertions.
         *
         * This can be useful to stop testing, when a condition has already failed,
         * and additional errors might just clutter the printed results.
         */
        PrevAssertionFailed,

        /**
         * When an unexpected/uncaught error occurs, for instance if
         * you try to access a ``std::optional``'s value, and it doesn't have one.
         */
        ExceptionCaught,
    };

    /**
     * Settings passed into the ``TestRunner`` and read by ``TestCase``.
     */
    struct Settings {
        /**
         * When one assertion fails, the following assertions in the same
         * scope can be cancelled. This is useful, if you want to avoid
         * cluttering the result output.
         */
        bool stopAssertingAfterFail = true;
    };

    /**
     * Container for test results of a single assertion.
     */
    struct TestResult {
        /**
         * If the assertion passed or not.
         */
        bool passed = false;

        /**
         * The assertions number within the ``it`` scope. Useful for
         * identifying exactly which assertion failed, when it might
         * not be clear from descriptions.
         */
        CaseNumber caseNo = 1;

        /**
         * Description shown in the result output (as provided by the ``it`` method)
         */
        std::string description;

        /**
         * Expected and actual results, provided as strings.
         */
        std::string expected, actual;

        /**
         * Additional information provided to a single assertion, using ``because``.
         */
        std::string additional;
    };

    /**
     * Container for error information.
     */
    struct Error {
        /**
         * A BBUnit-specific error code.
         */
        ErrorCode errorCode;

        /**
         * Error messages.
         * Can sometimes be decorated with ``additional`` information provided
         * by the ``because`` method.
         */
        std::string message, additional;
    };

    /**
     * Inspired by Rust's ``Result`` type, this ``std::variant``
     * provides either a ``TestResult`` or an ``Error``.
     *
     * It eases some of the syntax needed to interact with variants.
     */
    struct Result : public std::variant<TestResult, Error> {
        using std::variant<TestResult, Error>::variant;

        /**
         * Returns true, if the ``Result`` contains an ``Error``.
         * @return
         */
        [[nodiscard]] bool isErr() const noexcept(false) {
            return std::holds_alternative<Error>(*this);
        }

        /**
         * Returns the ``Error``.
         *
         * @important This has the potential to cause an error, if you
         *      haven't verified in advance that there actually is an error.
         *      You can use ``isErr`` for this.
         *
         * @return
         */
        [[nodiscard]] Error error() const noexcept(false) {
            return std::get<Error>(*this);
        }

        /**
         * Returns the ``TestResult``.
         *
         * @important This has the potential to cause an error, if you
         *      haven't verified in advance the content.
         *      You can use ``isErr`` for this, by verifying there isn't
         *      an error present.
         *
         * @return
         */
        [[nodiscard]] TestResult get() const noexcept(false) {
            return std::get<TestResult>(*this);
        }

    };

    /**
     * A container of multiple test results.
     */
    class TestResults : public std::vector<Result> {
    public:
        using std::vector<Result>::vector;

        /**
         * Easily append more results, on the form:
         *
         * ````cpp
         * currentResults += newResults;
         * ````
         *
         * @param other
         */
        void operator+=(const TestResults &other) {
            *this->insert(this->end(), other.begin(), other.end());
        }
    };

    /**
     * Trait to be used under ``TestCase``, whose primary purpose
     * is to define the available assertion methods, as well as
     * managing the assertion handling.
     */
    class ProvidesAssertions {
    public:
        /**
         * Used in the library's self-testing.
         */
        enum class Must {
            /**
             * The assertion must have passed.
             */
            HavePassed,

            /**
             * The assertion is expected to have failed.
             */
            HaveFailed,

            /**
             * The assertion is expected to have caused an error,
             * for example with trying to make assertions after a failed
             * assert, or by throwing errors (such as ``std::bad_optional_access``).
             */
            HaveCausedError,
        };

        /**
         * Provide additional descriptive information to a specific assertion.
         *
         * @param msg
         * @return
         */
        ProvidesAssertions &because(const std::string &msg) noexcept(false) {
            if (m_testResults.empty()) {
                return *this;
            }
            Result res = m_testResults[m_testResults.size() - 1];
            if (res.isErr()) {
                Error tmp = res.error();
                tmp.additional = msg;
                m_testResults[m_testResults.size() - 1] = tmp;
            } else {
                TestResult tmp = res.get();
                tmp.additional = msg;
                m_testResults[m_testResults.size() - 1] = tmp;
            }
            return *this;
        }

        /**
         * Used for the library's self-testing mechanism.
         *
         * It's appended to a specific assertion, and this method will then
         * verify the result the assertion has produced. Furthermore, this
         * method will adjust the results.
         *
         * For example, if an assertion is expected to have have a failed result.
         * Let's say, if it needs to check that two integers are equals, and
         * ``2`` and ``5`` were passed. In that case, the test fails.
         * However, if that is the expected outcome as part of the self-test,
         * this method will modify the ``TestResult`` to ensure correct reporting
         * in the test results. Because, from the point-of-view of BBUnit,
         * the test has actually passed.
         *
         * @param mustHave
         */
        void thisCase(Must mustHave) noexcept(false) {
            if (m_testResults.empty()) {
                return;
            }
            Result res = m_testResults[m_testResults.size() - 1];
            if (res.isErr()) {
                Error err = res.error();

                // When the test result holds an error, we will transform it to a ``TestResult``,
                // which checks if we expected an error in this location.
                m_testResults[m_testResults.size() - 1] = TestResult{
                        .passed = mustHave == Must::HaveCausedError,
                        .description = m_description,
                        .additional = err.additional,
                };
            } else {
                TestResult result = res.get();
                m_testResults[m_testResults.size() - 1] = TestResult{
                        .passed = (mustHave == Must::HavePassed && result.passed) || (mustHave == Must::HaveFailed && !result.passed),
                        .caseNo = result.caseNo,
                        .description = result.description,
                        .expected = result.expected,
                        .actual = result.actual,
                        .additional = result.additional,
                };
            }
        }

    protected:
        /**
         * Used for international communication between an assertion and
         * the assertion handling logic.
         */
        struct InternalResult {
            /**
             * If the assertion passed.
             */
            bool passed = false;

            /**
             * Expected and actual values (as string)
             */
            std::string expected, actual;
        };

        /**
         * Helper method which casts an "unknown" type T to a string.
         *
         * A few different methods are included here, since there's no
         * universal way of casting a value to a string.
         *
         * @tparam T
         * @param input
         * @return
         */
        template<typename T>
        std::string castToString(T input) const noexcept {
            if constexpr (std::is_convertible_v<T, std::string>) {
                return static_cast<std::string>(input);
            } else if constexpr (std::is_convertible_v<T, int> || std::is_convertible_v<T, float>) {
                return std::to_string(input);
            } else {
                return {};
            }
        }

        /**
         * Assert that two values are equal.
         *
         * @tparam T
         * @param expected
         * @param actual
         * @return
         */
        template<Comparable T>
        ProvidesAssertions &assertEquals(const T &expected, const T &actual) noexcept(false) {
            assert([&]() -> InternalResult {
                return {expected == actual, castToString(expected), castToString(actual)};
            });
            return *this;
        }

        /**
         * Assert that two values are not equal.
         *
         * @tparam T
         * @param expected
         * @param actual
         * @return
         */
        template<Comparable T>
        ProvidesAssertions &assertNotEquals(const T &expected, const T &actual) noexcept(false) {
            assert([&]() -> InternalResult {
                return {expected != actual, castToString(expected), castToString(actual)};
            });
            return *this;
        }

        /**
         * Assert the number of elements in an object match the expectation.
         *
         * The class must contain a ``size`` method (which returns ``size_t``).
         *
         * Typical subjects include ``std::vector``, ``std::map`` and ``std:.array``.
         *
         * @tparam T
         * @param expected
         * @param iter
         * @return
         */
        template<HasSizeMethod T>
        ProvidesAssertions &assertCount(size_t expected, const T &iter) noexcept(false) {
            assert([&]() -> InternalResult {
                return {expected == iter.size(),
                        std::to_string(static_cast<size_t>(expected)),
                        std::to_string(static_cast<size_t>(iter.size()))};
            });
            return *this;
        }

        /**
         * Assert that the subject satisfies the regular expression pattern.
         *
         * @param pattern
         * @param subject
         * @return
         */
        ProvidesAssertions &assertRegex(const std::string &pattern,
                                        const std::string &subject) noexcept(false) {
            assert([&]() -> InternalResult {
                return {std::regex_search(subject, std::regex(pattern)),
                        pattern,
                        subject};
            });
            return *this;
        }

        /**
         * Short-hand method to assert that a value is true.
         *
         * @param actual
         * @return
         */
        ProvidesAssertions &assertTrue(bool actual) noexcept(false) {
            assert([&]() -> InternalResult {
                return {actual, "true", "false"};
            });
            return *this;
        }

        /**
         * Short-hand method to assert that a value is false.
         * @param actual
         * @return
         */
        ProvidesAssertions &assertFalse(bool actual) noexcept(false) {
            assert([&]() -> InternalResult {
                return {!actual, "false", "true"};
            });
            return *this;
        }

        /**
         * Assert that an exception is thrown.
         *
         * Optionally, you can also verify that the exception contains a specific message.
         *
         * @tparam T
         * @param func
         * @param message
         * @return
         */
        template<HasWhatMethod T>
        ProvidesAssertions &assertException(const std::function<void()> &func,
                                            const std::optional<std::string> &message = std::nullopt) noexcept(false) {
            assert([&]() -> InternalResult {
                try {
                    func();
                } catch (T &e) {
                    return !message.has_value()
                                   ? InternalResult{true}
                                   : InternalResult{message.value() == e.what(), message.value(), e.what()};
                } catch (std::exception &e) {
                    // When another type of exception is thrown, we will attempt to
                    // get a hold of its name, so we can populate a more useful message
                    // to the result feed.
                    return {false, getClassName(T()), getClassName(e)};
                } catch (...) {
                    return {false, getClassName(T()), "<Unknown exception>"};
                }
                // No exception thrown
                return {false, getClassName(T()), "<No exception thrown>"};
            });
            return *this;
        }

        /**
         * Helper method to extract the class name of an object.
         *
         * The name is not always correctly formed, but it should provide
         * enough information to be useful. In any case, it's better than nothing :-)
         *
         * @tparam T
         * @param a
         * @return
         */
        template<typename T>
        [[nodiscard]] std::string getClassName(T a) const noexcept {
            return typeid(a).name();
        }

        /**
         * Start a new ``it`` scope (which can hold one or several assertions).
         *
         * @param description
         */
        void start(const std::string &description) noexcept {
            m_caseNo = 0;
            m_testResults.clear();
            m_description = description;
            m_assertionsActive = AssertionState::Started;
        }

        /**
         * End of ``it`` scope.
         */
        void end() noexcept {
            m_assertionsActive = AssertionState::NotStarted;
        }

        /**
         * Retrieve the computed test results.
         *
         * @return
         */
        [[nodiscard]] TestResults getResults() const noexcept {
            return m_testResults;
        }

        /**
         * Provide a set of settings to be used during the next
         * round of assertions.
         *
         * @param settings
         */
        void withSettings(const Settings &settings) noexcept {
            m_settings = settings;
        }

    private:
        /**
         * Used to keep track of whether an assertion has failed, and
         * if it has been requested to no longer perform assertions.
         */
        enum class AssertionState {
            /**
             * ``it`` scope not started.
             */
            NotStarted,

            /**
             * We have started testing assertions, and so far,
             * everything is fine -- or it hasn't been requested to stop
             * doing assertions when one fails.
             */
            Started,

            /**
             * A previous assertion has failed, and we want to stop
             * collecting them.
             */
            Paused,
        };

        /**
         * Settings, for instance specifying if we wish to continue performing
         * assertions after one has failed.
         */
        Settings m_settings;

        /**
         * The ``it`` scope's description/headline.
         */
        std::string m_description;

        /**
         * Current case number (within ``it`` scope)
         */
        CaseNumber m_caseNo = 0;

        /**
         * Temporary container of test results, which will be cleared
         * before/after starting a new round of assertions (i.e. entering an
         * ``it`` scope).
         */
        TestResults m_testResults;

        /**
         * Current state of assertions.
         */
        AssertionState m_assertionsActive = AssertionState::NotStarted;

        /**
         * An "internal" assert method which seeks to generalize as much as
         * of the assertion process as possible, for instance by handling
         * what happens when an assertion fails, in accordance with the settings.
         *
         * @param assertionFunc
         */
        inline void assert(const std::function<InternalResult()> &assertionFunc) noexcept(false) {
            if (m_assertionsActive != AssertionState::Started) {
                m_testResults.emplace_back(Error{
                        .errorCode = ErrorCode::PrevAssertionFailed,
                });
                return;
            }

            InternalResult result = assertionFunc();

            // If the test fails, and it has been requested to stop performing assertions,
            // we pause the assertions.
            if (!result.passed && m_settings.stopAssertingAfterFail) {
                m_assertionsActive = AssertionState::Paused;
            }

            m_testResults.emplace_back(TestResult{
                    .passed = result.passed,
                    .caseNo = ++m_caseNo,
                    .description = m_description,
                    .expected = result.expected,
                    .actual = result.actual,
            });
        }
    };

    /**
     * The inheritable ``TestCase`` which test cases must extend.
     */
    class TestCase : public ProvidesAssertions {
    public:
        /**
         * Run the test case with default settings.
         *
         * @return
         */
        virtual TestResults run() noexcept(false) final {
            return run({});
        }

        /**
         * Run the test case with explicitly defined settings.
         *
         * @param settings
         * @return
         */
        virtual TestResults run(const Settings settings) noexcept(false) final {
            withSettings(settings);
            test();
            return m_results;
        }

        /**
         * Abstract method which must be defined by the child class.
         * It contains one or more ``it`` scopes, and their respective assertions.
         */
        virtual void test() = 0;

    protected:
        /**
         * Perform operations, typically tests, while silencing the results.
         *
         * This is primarily useful for the library's self-testing.
         *
         * @param func
         * @return
         */
        TestResults whileSilent(const std::function<TestResults()> &func) noexcept(false) {
            m_silent = true;
            TestResults res = func();
            m_silent = false;
            return res;
        }

        /**
         * Create a group of assertions (internally referred to as "``it`` scope").
         *
         * @param description
         * @param userAssertsThat
         * @return
         */
        TestResults it(const std::string &description,
                       const std::function<void()> &userAssertsThat) noexcept(false) {
            start(description);
            TestResults newResults;

            // We encapsulate the function in a try/catch block to catch unintended
            // errors. If we didn't do this, a "simple" error like ``std::bad_optional_access``
            // could kill the entire test execution. Instead, we catch it here, and
            // show it in the result sheet that this error occured.
            try {
                userAssertsThat();
                newResults = getResults();
            } catch (const std::exception &e) {
                newResults.emplace_back(Error{ErrorCode::ExceptionCaught, e.what()});
            } catch (...) {
                newResults.emplace_back(Error{ErrorCode::ExceptionCaught, "Unknown exception."});
            }

            if (!m_silent) {
                m_results.insert(m_results.end(), newResults.begin(), newResults.end());
            }

            end();

            return newResults;
        }

    private:
        /**
         * Silence results. Typically only used in the library's self-test.
         */
        bool m_silent = false;

        /**
         * Accumulated test results across all ``it`` scopes.
         */
        TestResults m_results;
    };

    /**
     * Responsible for running the list of test cases, and collecting their results.
     */
    class TestRunner {
    public:
        static TestResults run(const std::vector<std::shared_ptr<TestCase>> &testCases) noexcept(false) {
            TestResults result;
            std::for_each(testCases.begin(),
                          testCases.end(),
                          [&](const std::shared_ptr<TestCase> &testCase) {
                              result += testCase->run();
                          });

            return result;
        }
    };
}
