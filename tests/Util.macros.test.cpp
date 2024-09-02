/* Util.test.cpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "test-utils/common.hpp"

#include "../include/util/macros.hpp"

#include <iterator>
#include <sstream>
#include <string>

#define PRINT_MACRO(x) buffer << #x << ": " << x << ", " << std::flush;

enum Colors { Red, Green, Blue };

#define NAME_ENTRY(field) NAMED_PAIR(field),

BEGIN_TEST_SUITE("Util.Macros")
{
	TEST_CASE("FOREACH_PARAM Macro works")
	{
		std::stringstream buffer;
		int field1 = 10;
		int field2 = 20;

		FOREACH_PARAM(PRINT_MACRO, field1, field2);
		REQUIRE(buffer.str() == "field1: 10, field2: 20, ");
	}

	TEST_CASE("FOREACH_ENUM_PARAM Macro works")
	{
		std::pair<const std::string, Colors> color_pairs[] = {
			FOREACH_PARAM(NAME_ENTRY, Red, Green, Blue)
		};

		CHECK(std::size(color_pairs) == 3);
		REQUIRE(color_pairs[0].first == "Red");
		REQUIRE(color_pairs[1].first == "Green");
		REQUIRE(color_pairs[2].first == "Blue");
	}
}

// clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
