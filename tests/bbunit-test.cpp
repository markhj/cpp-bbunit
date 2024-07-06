#include <bbunit/bbunit.hpp>
#include <map>
#include <optional>

namespace BBUnit::Tests {
    class BBUnitTest : public TestCase {
    public:
        /**
         * Collection of all tests.
         */
        void test() override {
            equals();
            shorthands();
            count();
            mustCausePrevAssertionFailed();
            expectedActual();
            enums();
            exceptions();
            regex();
            catchUnintendedErrors();
        }

        /**
         * Here, we mainly test ``assertEquals``.
         *
         * ``assertEquals`` is template-based, and therefore -- theoretically -- supports
         * an almost unlimited number of types. Therefore, some common ones are selectively
         * chosen for this test.
         */
        void equals() {
            it("Assert that integers are equal", [&]() {
                assertEquals<int>(2, 2).thisCase(Must::HavePassed);
                assertEquals<int>(2, 3).thisCase(Must::HaveFailed);
            });

            it("Assert that floats are equal", [&]() {
                assertEquals<float>(2.5, 2.5).thisCase(Must::HavePassed);
                assertEquals<float>(2.5, 3.5).thisCase(Must::HaveFailed);
            });

            it("Assert that chars are equal", [&]() {
                assertEquals<char>('a', 'a').thisCase(Must::HavePassed);
                assertEquals<char>('a', 'b').thisCase(Must::HaveFailed);
            });

            it("Assert that strings are equal", [&]() {
                assertEquals<std::string>("foo", "foo").thisCase(Must::HavePassed);
                assertEquals<std::string>("foo", "bar").thisCase(Must::HaveFailed);
            });

            // Ensure that a custom class, and it's comparison overload will function
            // properly with BBUnit.
            it("Assert that classes/structs are equal", [&]() {
                struct A {
                    int x = 0;
                    bool operator==(const A &other) const {
                        return x == other.x;
                    }
                };

                A one, two, three;
                three.x = 10;

                assertEquals<A>(one, two).thisCase(Must::HavePassed);
                assertEquals<A>(one, three).thisCase(Must::HaveFailed);
            });
        }

        /**
         * Test of short-hand methods, which are methods that don't really do much
         * on their own. They just provide convenient shortcuts.
         */
        void shorthands() {
            it("assertTrue", [&]() {
                assertTrue(true).thisCase(Must::HavePassed);
                assertTrue(false).thisCase(Must::HaveFailed);
            });

            it("assertFalse", [&]() {
                assertFalse(false).thisCase(Must::HavePassed);
                assertFalse(true).thisCase(Must::HaveFailed);
            });

            it("Assert that integers are not equal", [&]() {
                assertNotEquals<int>(2, 8).thisCase(Must::HavePassed);
                assertNotEquals<int>(2, 2).thisCase(Must::HaveFailed);
            });
        }

        /**
         * Verify that we stop performing assertions (upon request via ``Settings``),
         * when a previous case has failed.
         *
         * In this example, case 2 fails, which means case 3 should contain an error.
         */
        void mustCausePrevAssertionFailed() {
            it("The third case must cause error, because case 2 fails", [&]() {
                assertEquals<int>(2, 2).thisCase(Must::HavePassed);
                assertEquals<int>(2, 3).thisCase(Must::HaveFailed);
                assertEquals<int>(2, 2).thisCase(Must::HaveCausedError);
            });
        }

        /**
         * Verify that "expected" and "actual" get printed when possible.
         *
         * Test performed in a number of common formats.
         */
        void expectedActual() {
            TestResults general = whileSilent([&]() -> TestResults {
                return it("", [&]() {
                    assertNotEquals<std::string>("a", "b");
                    assertNotEquals<int>(2, 23);
                    assertNotEquals<unsigned int>(2, 23);
                    assertNotEquals<float>(5.5, 6.5);
                    assertNotEquals<double>(5.5, 6.5);
                });
            });

            it("Test that expected and actual are reported", [&]() {
                // std::string
                assertEquals<std::string>("a", general[0].get().expected);
                assertEquals<std::string>("b", general[0].get().actual);

                // int
                assertEquals<std::string>("2", general[1].get().expected);
                assertEquals<std::string>("23", general[1].get().actual);

                // unsigned int
                assertEquals<std::string>("2", general[2].get().expected);
                assertEquals<std::string>("23", general[2].get().actual);

                // float
                assertEquals<std::string>("5.5", general[3].get().expected.substr(0, 3));
                assertEquals<std::string>("6.5", general[3].get().actual.substr(0, 3));

                // double
                assertEquals<std::string>("5.5", general[4].get().expected.substr(0, 3));
                assertEquals<std::string>("6.5", general[4].get().actual.substr(0, 3));
            });
        }

        /**
         * Ensure that BBUnit catches (unintended) errors, such as accessing
         * the value of a ``std::optional`` without any.
         *
         * When that happens, an error should be produced as result.
         */
        void catchUnintendedErrors() {
            TestResults res = whileSilent([&]() -> TestResults {
                return it("", [&]() {
                    std::optional<int> x;
                    x.value();
                });
            });

            it("Catch unintended errors and avoid interruption", [&]() {
                assertEquals(true, res[0].isErr());
            });
        }

        /**
         * Check that ``assertCount`` works on a number of intended target types.
         *
         * Like ``assertEquals``, it's not realistic to cover _every_ use-case,
         * because ``assertCount`` relies on a concept (which requires a ``size`` method
         * which returns ``size_t``).
         */
        void count() {
            it("Counts vector elements", [&]() {
                assertCount(2, std::vector<int>({1, 2})).thisCase(Must::HavePassed);
                assertCount(2, std::vector<int>({1, 2, 3})).thisCase(Must::HaveFailed);
            });

            it("Counts map elements", [&]() {
                std::map<int, int> map = {{1, 2}, {2, 3}};
                assertCount(2, map).thisCase(Must::HavePassed);
                assertCount(3, map).thisCase(Must::HaveFailed);
            });
        }

        /**
         * Check that we can assert enumerators.
         */
        void enums() {
            it("Assert enums", [&]() {
                enum class A { X,
                               Y };

                assertEquals<A>(A::X, A::X).thisCase(Must::HavePassed);
                assertEquals<A>(A::X, A::Y).thisCase(Must::HaveFailed);
            });
        }

        /**
         * Check the regular expression assertion.
         */
        void regex() {
            it("Tests regular expressions", [&]() {
                assertRegex(R"(^\d+$)", "123123").thisCase(Must::HavePassed);
                assertRegex(R"(^\d+$)", "123abc").thisCase(Must::HaveFailed);
            });
        }

        /**
         * Various test on the exception catching.
         */
        void exceptions() {
            // The expected exception (type) is caught. In this example, we
            // expected ``std::bad_optional_access`` to be thrown, intentionally.
            assertException<std::bad_optional_access>([]() {
                std::optional<int> x;
                x.value();
            }).thisCase(Must::HavePassed);

            // When we expected an exception, but nothing has been thrown,
            // the case must be considered failed.
            it("Expected exception, but nothing thrown", [&]() {
                assertException<std::bad_optional_access>([]() {

                }).thisCase(Must::HaveFailed);
            });

            // When we expect an exception, and one is thrown, but it's the wrong kind.
            it("Wrong exception thrown", [&]() {
                assertException<std::bad_optional_access>([]() {
                    throw std::runtime_error("");
                }).thisCase(Must::HaveFailed);
            });

            // When the exception is of the correct type, and we want to check
            // that it contains the correct message.
            it("Exception has right type and a right/wrong message", [&]() {
                assertException<std::bad_optional_access>([]() {
                    std::optional<int>().value();
                },
                                                          "bad optional access")
                        .thisCase(Must::HavePassed);

                assertException<std::bad_optional_access>([]() {
                    std::optional<int>().value();
                },
                                                          "Goodbye, World!")
                        .thisCase(Must::HaveFailed);
            });

            // And lastly, checking that the correct message, but wrong exception type,
            // doesn't accidentally produce a positive result.
            it("Exception has wrong type and a right/wrong message", [&]() {
                assertException<std::bad_alloc>([]() {
                    std::optional<int>().value();
                },
                                                "bad optional access")
                        .thisCase(Must::HaveFailed);
            });
        }
    };
}
