/* Util.toml.test.cpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <toml++/toml.h>
#include <unordered_map>

#include "test-utils/common.hpp"

#include "tomlize.hpp"

BEGIN_TEST_SUITE("Util.Toml")
{
	enum Colors { Red, Green, Blue };

	struct SimpleStruct {
		int field1;
		char field2;
	};
	TOMLIZE_STRUCT(SimpleStruct, field1, field2);

	struct SimpleStruct2 {
		int fieldA;
		int fieldB;
	};

	struct StructWithEnum {
		int field1;
		int field2;
		Colors foreground;
	};

	struct CompositeStruct {
		SimpleStruct part1;
		SimpleStruct2 part2;
	};

	struct ComplexStruct {
		SimpleStruct part1;
		SimpleStruct2 part2;
		StructWithEnum part3;
		Colors background;
	};

	TEST_CASE("SimpleStruct Serailizes")
	{
		SimpleStruct s{ 1 };
		toml::table t = TOML::to_toml(s);

		CHECK(t.at("field1").as_integer() == 1);
	}
}
// clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
