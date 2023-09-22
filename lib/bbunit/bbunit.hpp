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

        void assertEquals(int expected, int actual, const char *message)
        {
            assert(expected == actual, message);
        }

        void assertEquals(double expected, double actual, const char *message)
        {
            assert(expected == actual, message);
        }

        void assertEquals(bool expected, bool actual, const char *message)
        {
            assert(expected == actual, message);
        }

        void assertEquals(const std::string& expected, const std::string& actual, const char *message)
        {
            assert(expected == actual, message);
        }

        void assertGreaterThan(int compare, int actual, const char *message)
        {
            assert(actual > compare, message);
        }

        void assertGreaterThan(double compare, double actual, const char *message)
        {
            assert(actual > compare, message);
        }

        void assertTrue(bool actual, const char* message)
        {
            assert(actual, message);
        }

        void assertFalse(bool actual, const char* message)
        {
            assert(!actual, message);
        }

        template <typename T>
        void assertCount(unsigned int expected, std::vector<T> vector, const char* message)
        {
            assert(expected == vector.size(), message);
        }

        template <typename T>
        void assertEmpty(std::vector<T> vector, const char* message)
        {
            assert(vector.size() == 0, message);
        }

        template <typename K, typename V>
        void assertCount(unsigned int expected, std::map<K, V> map, const char* message)
        {
            assert(expected == map.size(), message);
        }

        template <typename K, typename V>
        void assertEmpty(std::map<K, V> map, const char* message)
        {
            assert(map.size() == 0, message);
        }

    private:
        unsigned int assertions = 0,
            passed = 0;

        void assert(bool testPassed, const std::string& message)
        {
            assertions++;

            setColor(FOREGROUND_RESET | BACKGROUND_RESET);

            if (testPassed) {
                passed++;
                setColor(FOREGROUND_GREEN);
                std::cout << "  OK  ";
            } else {
                setColor(BACKGROUND_RED);
                std::cout << " FAIL ";
            }

            setColor(FOREGROUND_RESET | BACKGROUND_RESET);
            std::cout << " " << message << std::endl;
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

            std::cout << "\n============================"
                << "\nPassed: " << passed << " | Total: " << assertions << std::endl;
        }
    };

}

#endif
