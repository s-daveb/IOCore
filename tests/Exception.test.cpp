/* exceptions-test.cpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "Exception.hpp"

#include "test-utils/common.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include <exception>
#include <iostream>
#include <optional>
#include <stdexcept>

namespace Match = Catch::Matchers;
using Catch::CaseSensitive;

void throw_wrapped_stl_exception()
{
	auto local = std::logic_error("This is a stack variable");

	throw core::Exception(local);
}

BEGIN_TEST_SUITE("core::Exception")
{
	auto throw_an_exception = []() {
		throw core::Exception("An error occurred!!");
	};

	TEST("core::Exception -  Can throw new exception type")
	{
		REQUIRE_THROWS(throw_an_exception());
	}

	TEST("core::Exception -  Can construct exception various ways")
	{
		SECTION("1. Blank constructor")
		{
			core::Exception obj;
		}
		SECTION("2a. With cstring parameter")
		{
			core::Exception obj("Sample Error");
		}
		SECTION("2b. With std::string parameter")
		{
			core::Exception obj(std::string("Sample Error"));
		}

		SECTION("3. With STL exception")
		{
			core::Exception obj(std::runtime_error("Sample Error"));
		}
		SECTION("4. With destroyed stack")
		{
			auto nested_function_call = []() {
				throw_wrapped_stl_exception();
			};
			try {
				nested_function_call();
			} catch (std::exception& e) {
				REQUIRE_THAT(
				    e.what(),
				    Match::ContainsSubstring(
					"This is a stack variable"
				    )
				);
			}
		}
	}

	TEST("core::Exception::what() - message reflects error")
	{

		SECTION("1. Unspecified error or exception")
		{
			core::Exception obj;

			REQUIRE_THAT(
			    obj.what(),
			    Match::ContainsSubstring(
				core::Exception::kDefault_Error,
				CaseSensitive::Yes
			    )
			);
		}

		SECTION("2. custom error or exception")
		{
			constexpr auto kTEST_MESSAGE = "This is a test.";
			core::Exception test_object_one(kTEST_MESSAGE);
			core::Exception test_object_two(
			    std::logic_error("Makes no sense")
			);
			SECTION(" a: what() does not contain default message")
			{
				REQUIRE_THAT(
				    test_object_one.what(),
				    !Match::ContainsSubstring(
					core::Exception::kDefault_Error
				    )
				);
			}
			SECTION(" b: what() displays custom message")
			{
				REQUIRE_THAT(
				    test_object_one.what(),
				    Match::ContainsSubstring(kTEST_MESSAGE)
				);
			}
			SECTION(" c: what() contains inner exception message")
			{
				REQUIRE_THAT(
				    test_object_two.what(),
				    Match::ContainsSubstring("Makes no sense")
				);
			}
		}
	}

	TEST("core::Exception::what() - contains stacktrace with Catch2 "
	     "runtime method names")
	{
		core::Exception test_object("Test");
		SECTION(" a: what() does not contain default message")
		{
			REQUIRE_THAT(
			    test_object.what(),
			    Match::ContainsSubstring("Catch::RunContext")
			);
			SUCCEED(test_object.what());
		}
	}
}

// clang-format off
// vim: set foldmethod=syntax  textwidth=80 ts=4 sts=0 sw=4  noexpandtab ft=cpp.doxygen :
