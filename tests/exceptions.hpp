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
        }, "assertException when exception is thrown"));

        assertFalse(assertException([]() {
        }, "assertException when no exception thrown"));

        assertTrue(assertException([]() {
            throw std::runtime_error("Some error");
        }, "assertException when runtime_error is thrown"));
    }

    void exceptionsOfType()
    {
        assertTrue(assertException<CustomException>([]() {
            throw CustomException();
        }, "CustomException thrown and expected"));

        assertFalse(assertException<CustomException>([]() {
            throw std::exception();
        }, "Normal exception thrown, but CustomException expected"));
    }

    void exceptionMessages()
    {
        // Const char
        assertTrue(assertExceptionMessage("Some error", []() {
            throw std::runtime_error("Some error");
        }, "assertExceptionMessage with correct message (const char)"));

        assertFalse(assertExceptionMessage("Incorrect", []() {
            throw std::runtime_error("Some error");
        }, "assertExceptionMessage with wrong message (const char)"));

        // std::string
        assertTrue(assertExceptionMessage(std::string("Hello"), []() {
            throw std::runtime_error("Hello");
        }, "assertExceptionMessage with correct message (string)"));

        assertFalse(assertExceptionMessage(std::string("Incorrect"), []() {
            throw std::runtime_error("Hello");
        }, "assertExceptionMessage with wrong message (string)"));

        // By type (const char)
        assertTrue(assertExceptionMessage<CustomException>("Some error", []() {
            throw CustomException("Some error");
        }, "assertExceptionMessage with correct message (const char)"));

        assertFalse(assertExceptionMessage<CustomException>("Some error", []() {
            throw std::runtime_error("Some error");
        }, "assertExceptionMessage with correct message but wrong type (const char)"));

        // By type (string)
        assertTrue(assertExceptionMessage<CustomException>(std::string("Some error"), []() {
            throw CustomException("Some error");
        }, "assertExceptionMessage with correct message (string)"));

        assertFalse(assertExceptionMessage<CustomException>(std::string("Some error"), []() {
            throw std::runtime_error("Some error");
        }, "assertExceptionMessage with correct message but wrong type (string)"));

        assertFalse(assertExceptionMessage<CustomException>(std::string("False message"), []() {
            throw CustomException("Some error");
        }, "assertExceptionMessage with right type, but wrong content (string)"));
    }

    void exceptionMessageContainsConstChar()
    {
        assertTrue(assertExceptionMessageContains("world", []() {
            throw std::runtime_error("Hello world");
        }, "assertExceptionMessageContains with correct content (const char)"));

        assertFalse(assertExceptionMessageContains("Incorrect", []() {
            throw std::runtime_error("Hello world");
        }, "assertExceptionMessageContains with wrong content (const char)"));

        assertTrue(assertExceptionMessageContainsCI("WORLD", []() {
            throw std::runtime_error("Hello world");
        }, "assertExceptionMessageContains with correct content (const char)"));

        assertFalse(assertExceptionMessageContainsCI("INCORRECT", []() {
            throw std::runtime_error("Hello world");
        }, "assertExceptionMessageContains with wrong content (const char)"));

        assertTrue(assertExceptionMessageContains<std::runtime_error>("world", []() {
            throw std::runtime_error("Hello world");
        }, "assertExceptionMessageContains with correct content (const char)"));

        assertFalse(assertExceptionMessageContains<CustomException>("world", []() {
            throw std::runtime_error("Hello world");
        }, "assertExceptionOfTypeContains with right content but wrong class (const char)"));

        assertTrue(assertExceptionMessageContainsCI<std::runtime_error>("WORLD", []() {
            throw std::runtime_error("Hello world");
        }, "assertExceptionMessageContains with correct content (const char)"));

        assertFalse(assertExceptionMessageContainsCI<CustomException>("WORLD", []() {
            throw std::runtime_error("Hello world");
        }, "assertExceptionOfTypeContains with right content but wrong class (const char)"));
    }

    void exceptionMessageContainsString()
    {
        assertTrue(assertExceptionMessageContains(std::string("world"), []() {
            throw std::runtime_error("Hello world");
        }, "assertExceptionMessageContains with correct content (const char)"));

        assertFalse(assertExceptionMessageContains(std::string("Incorrect"), []() {
            throw std::runtime_error("Hello world");
        }, "assertExceptionMessageContains with wrong content (const char)"));

        assertTrue(assertExceptionMessageContains<std::runtime_error>(std::string("world"), []() {
            throw std::runtime_error("Hello world");
        }, "assertExceptionMessageContains with correct content (const char)"));

        assertFalse(assertExceptionMessageContains<CustomException>(std::string("world"), []() {
            throw std::runtime_error("Hello world");
        }, "assertExceptionOfTypeContains with right content but wrong class (const char)"));
    }

    void regexExceptions()
    {
        assertTrue(
                assertExceptionMessageRegex(R"(^Hello \d+$)", []() {
                    throw std::runtime_error("Hello 123");
                }, "Exception with valid regex")
        );

        assertFalse(
                assertExceptionMessageRegex(R"(^Hello \d+$)", []() {
                    throw std::runtime_error("Hello world");
                }, "Exception with invalid regex")
        );

        assertTrue(
                assertExceptionMessageRegex<CustomException>(R"(^Hello \d+$)", []() {
                    throw CustomException("Hello 123");
                }, "Exception with valid regex")
        );

        assertFalse(
                assertExceptionMessageRegex<CustomException>(R"(^Hello \d+$)", []() {
                    throw std::runtime_error("Hello 123");
                }, "Exception with valid regex")
        );
    }
};


#endif
