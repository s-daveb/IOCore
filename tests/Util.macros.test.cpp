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
#include "../include/util/toml.hpp"

#include <sstream>

#define PRINT_MACRO(x) buffer << #x << ": " << x << ", " << std::flush;

BEGIN_TEST_SUITE("Util.Macros")
{
	TEST_CASE("IOCORE_FOREACH Macro works")
	{
		std::stringstream buffer;
		int field1 = 10;
		int field2 = 20;

		IOCORE_FOREACH(PRINT_MACRO, field1, field2)
		REQUIRE(buffer.str() == "field1: 10, field2: 20, ");
	}
}

// clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
