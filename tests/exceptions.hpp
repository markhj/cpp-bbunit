#ifndef BBUNIT_TEST_EXCEPTIONS_HPP
#define BBUNIT_TEST_EXCEPTIONS_HPP

#include "bbunit.hpp"

class CustomException : public std::exception {
public:
    CustomException() : msg("Default message") {}

    explicit CustomException(const char *message) : msg(message) { }

    [[nodiscard]] const char* what() const noexcept override {
        return msg;
    }
private:
    const char *msg;
};

class ExceptionsTest : public BBUnit::TestCase {
public:
    ExceptionsTest() {
        setMode(BBUnit::TestMode::SelfTest);
    }

    void exceptions()
    {
        assertTrue(assertException([]() {
            throw std::exception();
        }));

        assertFalse(assertException([]() {
        }, "Assert that no exception is thrown"));
    }

    void exceptionsOfType()
    {
        assertTrue(assertException<CustomException>([]() {
            throw CustomException();
        }));

        assertFalse(assertException<CustomException>([]() {
            throw std::exception();
        }));
    }

    void exceptionMessages()
    {
        // Const char
        assertTrue(assertExceptionMessage("Some error", []() {
            throw std::runtime_error("Some error");
        }));

        assertFalse(assertExceptionMessage("Incorrect", []() {
            throw std::runtime_error("Some error");
        }));

        // std::string
        assertTrue(assertExceptionMessage(std::string("Hello"), []() {
            throw std::runtime_error("Hello");
        }));

        assertFalse(assertExceptionMessage(std::string("Incorrect"), []() {
            throw std::runtime_error("Hello");
        }));

        // By type (const char)
        assertTrue(assertExceptionMessage<CustomException>("Some error", []() {
            throw CustomException("Some error");
        }));

        assertFalse(assertExceptionMessage<CustomException>("Some error", []() {
            throw std::runtime_error("Some error");
        }));

        // By type (string)
        assertTrue(assertExceptionMessage<CustomException>(std::string("Some error"), []() {
            throw CustomException("Some error");
        }));

        assertFalse(assertExceptionMessage<CustomException>(std::string("Some error"), []() {
            throw std::runtime_error("Some error");
        }));

        assertFalse(assertExceptionMessage<CustomException>(std::string("False message"), []() {
            throw CustomException("Some error");
        }));
    }

    void exceptionMessageContainsConstChar()
    {
        assertTrue(assertExceptionMessageContains("world", []() {
            throw std::runtime_error("Hello world");
        }));

        assertFalse(assertExceptionMessageContains("Incorrect", []() {
            throw std::runtime_error("Hello world");
        }));

        assertTrue(assertExceptionMessageContainsCI("WORLD", []() {
            throw std::runtime_error("Hello world");
        }));

        assertFalse(assertExceptionMessageContainsCI("INCORRECT", []() {
            throw std::runtime_error("Hello world");
        }));

        assertTrue(assertExceptionMessageContains<std::runtime_error>("world", []() {
            throw std::runtime_error("Hello world");
        }));

        assertFalse(assertExceptionMessageContains<CustomException>("world", []() {
            throw std::runtime_error("Hello world");
        }));

        assertTrue(assertExceptionMessageContainsCI<std::runtime_error>("WORLD", []() {
            throw std::runtime_error("Hello world");
        }));

        assertFalse(assertExceptionMessageContainsCI<CustomException>("WORLD", []() {
            throw std::runtime_error("Hello world");
        }));
    }

    void exceptionMessageContainsString()
    {
        assertTrue(assertExceptionMessageContains(std::string("world"), []() {
            throw std::runtime_error("Hello world");
        }));

        assertFalse(assertExceptionMessageContains(std::string("Incorrect"), []() {
            throw std::runtime_error("Hello world");
        }));

        assertTrue(assertExceptionMessageContains<std::runtime_error>(std::string("world"), []() {
            throw std::runtime_error("Hello world");
        }));

        assertFalse(assertExceptionMessageContains<CustomException>(std::string("world"), []() {
            throw std::runtime_error("Hello world");
        }));
    }

    void regexExceptions()
    {
        assertTrue(
                assertExceptionMessageRegex(R"(^Hello \d+$)", []() {
                    throw std::runtime_error("Hello 123");
                })
        );

        assertFalse(
                assertExceptionMessageRegex(R"(^Hello \d+$)", []() {
                    throw std::runtime_error("Hello world");
                })
        );

        assertTrue(
                assertExceptionMessageRegex<CustomException>(R"(^Hello \d+$)", []() {
                    throw CustomException("Hello 123");
                })
        );

        assertFalse(
                assertExceptionMessageRegex<CustomException>(R"(^Hello \d+$)", []() {
                    throw std::runtime_error("Hello 123");
                })
        );
    }
};


#endif
