/**
* C++ BBUnit - Printer utility
*
* This ``Printer`` is the default "out-of-the-box" result visualizer
* for C++ BBUnit.
*/

#pragma once

#include <cassert>

#ifdef _WIN32
#include <windows.h>
#endif

namespace BBUnit::Utilities {
    /**
    * Printer settings
    */
    struct PrinterSettings {
        /**
        * When true, passed tests will also be explicitly shown in the
        * test results. This is not recommended for larger test suites,
        * as it clutters the results.
        */
        bool printPassed = false;

        /**
         * When true, "Previous case failed" will not be printed to the
         * results.
         */
        bool silencePrevAssertionFailed = true;
    };

    class Printer {
    public:
        /**
        * Print the test results with the specified settings.
        *
        * @param results
        * @param settings
        */
        static void print(const TestResults &results,
                          const PrinterSettings &settings) {
            // Keep track of passed, failed and erroneous assertions for the summary
            uint16_t passed = 0, failed = 0, errors = 0;

            // Shameless self-promotion...
            std::cout << "C++ BBUnit" << std::endl;

            std::for_each(results.begin(), results.end(), [&](const Result &result) {
                if (result.isErr()) {
                    ++errors;

                    Error err = std::get<Error>(result);

                    if (settings.silencePrevAssertionFailed && err.errorCode == ErrorCode::PrevAssertionFailed) {
                        return;
                    }

                    if (!settings.printPassed) {
                        std::cout << "\n";
                    }

                    setTextFormat(Color::Red);
                    std::cout << " ERR  ";
                    setTextFormat(Color::Blank, true);

                    std::cout << " " << err.info.description << "\n" << strRepeat(6, ' ');
                    if (!err.info.additional.empty()) {
                        std::cout << " - " << err.info.additional;
                    }

                    setTextFormat(Color::Blank);

                    // Convert the error code into a human-readable message
                    switch (err.errorCode) {
                        case ErrorCode::PrevAssertionFailed:
                            std::cout << " Previous case failed";
                            break;
                        case ErrorCode::ExceptionCaught:
                            std::cout << " Exception caught";
                            break;
                        default:
                            // This is a message to developers of BBUnit :-)
                            // And in a perfect world, this never happens, because it would
                            // indicate that something hasn't been properly tested on our end
                            assert(false && "Mapping of error codes is incomplete.");
                    }

                    if (!err.message.empty()) {
                        std::cout << ": " << err.message;
                    };

                    if (!err.info.additional.empty()) {
                        std::cout << " >> " << err.info.additional;
                    }

                    std::cout << std::endl;
                } else {
                    auto testResult = std::get<TestResult>(result);

                    if (testResult.passed) {
                        ++passed;
                    } else {
                        ++failed;
                    }

                    // If we don't want to print passed assertions, we skip ahead
                    if (testResult.passed && !settings.printPassed) {
                        return;
                    }

                    // If we don't print passed assertions, we add some whitespace
                    // to make it easier to read the errors.
                    if (!settings.printPassed) {
                        std::cout << "\n";
                    }

                    // Box with either "PASS" or "FAIL"
                    setTextFormat(testResult.passed ? Color::Green : Color::Red);
                    std::cout << (testResult.passed ? " PASS " : " FAIL ");

                    setTextFormat(Color::Blank, true);

                    // Print the description and the assertion's case number (e.g. if it's the 3rd assertion
                    // in the scope)
                    std::cout << " " << testResult.info.description << " ";
                    std::cout << "#" << std::to_string(testResult.info.caseNo);

                    if (!testResult.info.additional.empty()) {
                        std::cout << " - " << testResult.info.additional;
                    }

                    setTextFormat(Color::Blank);

                    if (!testResult.passed) {
                        printExpectedActual(testResult.expected, testResult.actual);
                    }

                    std::cout << "\n";
                }
            });

            printSummary(passed, failed, errors);
        }

    private:
        /**
         * Helper function to manage printing of expected and actual values.
         *
         * @param expected
         * @param actual
         */
        static inline void printExpectedActual(const std::string &expected,
                                               const std::string &actual) {
            auto indent = "\n" + strRepeat(7, ' ');
            if (expected.length() > 12 && actual.length() > 12) {
                std::cout << indent << "Expected: " << parseResultValue(expected);
                std::cout << indent << "Actual  : " << parseResultValue(actual);
            } else {
                std::cout << indent;
                std::cout << "Expected: " << parseResultValue(expected);
                std::cout << ", Actual: " << parseResultValue(actual);
            }
        }

        /**
         * Helper method to parse expected and actual outputs.
         *
         * @param input
         * @return
         */
        static inline std::string parseResultValue(const std::string& input) {
            return input.empty() ? "<Empty>" : input;
        }

        /**
        * Available color schemes for console output.
        */
        enum class Color {
            Blank,
            Green,
            Red,
        };

        /**
        * Create a string that consists of X characters.
        *
        * @param len
        * @param c
        * @return
        */
        static std::string strRepeat(uint8_t len, char c) {
            std::string s;
            while (s.length() < len) {
                s += c;
            }
            return s;
        }

        /**
        * Append character ``c`` until the text has the desired size.
        *
        * @param target
        * @param size
        * @param c
        * @return
        */
        static std::string pad(const std::string &target, uint8_t size, char c = ' ') {
            if (target.length() > size) {
                return target.substr(0, size);
            }
            return target + strRepeat(size - target.length(), c);
        }

        /**
        * Print the summarized results.
        *
        * @param passed
        * @param failed
        * @param errors
        */
        static void printSummary(uint16_t passed, uint16_t failed, uint16_t errors) {
            // Max. size of cells, such as "Total: X".
            // Just to pad spacing for nice and aligned look in the summary.
            uint8_t cellSize = 14;

            // Divider
            std::cout << "\n" + strRepeat(cellSize * 4 + 16, '-') + "\n";

            // Print a green or red indicator for whether there were any failed tests
            if (!failed && !errors) {
                setTextFormat(Color::Green);
                std::cout << " NICE ";
            } else {
                setTextFormat(Color::Red);
                std::cout << " FAIL ";
            }

            setTextFormat(Color::Blank);

            if (!failed && !errors) {
                std::cout << " Assertions passed: " << std::to_string(passed);
            } else {
                std::cout << " " << pad("Total: " + std::to_string(passed + failed + errors), cellSize);
                std::cout << " | " << pad("Passed: " + std::to_string(passed), cellSize);
                std::cout << " | " << pad("Failed: " + std::to_string(failed), cellSize);
                std::cout << " | " << pad("Errors: " + std::to_string(errors), cellSize);
            }

            std::cout.flush();
        }

        /**
        * Set the console output to be a specified background and text color.
        *
        * @note Currently only Windows support is added. However, the "consequence"
        *      on other platforms is simply that the results are printed in all white.
        *
        * @param color
        */
        void static setTextFormat(Color color, bool bold = false) {
#ifdef _WIN32
            WORD attr;
            switch (color) {
                case Color::Green:
                    attr = 0x002F;
                    break;
                case Color::Red:
                    attr = 0x004F;
                    break;
                default:
                    attr = bold ? 0x000F : 0x0007;
            }
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), attr);
#endif
        }
    };
}
