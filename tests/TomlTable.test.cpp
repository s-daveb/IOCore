/* Util.toml.test.cpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <unordered_map>

#include "test-utils/common.hpp"

#include "../include/TomlTable.hpp"
#include "../include/util/toml.hpp"

enum Colors { Red, Green, Blue };

IOCORE_TOML_ENUM(Colors, Red, Green, Blue);
//  IOCORE_TOML_ENUM(Colors, { Red, "Red" }, { Green, "Green" }, { Blue, "Blue"
//  });

using IOCore::TomlTable;

BEGIN_TEST_SUITE("IOCore.TomlTable")
{
	struct SimpleStruct {
		int field1;
		int field2;

		IOCORE_TOML_SERIALIZABLE(SimpleStruct, field1, field2);
	};

	struct ComplexStruct {
		int field1;
		int field2;
		Colors foreground;

		IOCORE_TOML_SERIALIZABLE(
		    ComplexStruct, field1, field2, foreground
		);
	};

	TEST_CASE("Toml::Table class construction and basic operators")
	{
		auto data = ComplexStruct{ 11, 22, Blue };
		TomlTable table = data;
		auto newdest = table.as<ComplexStruct>();

		CHECK(table.size() == 3);

		CHECK(table["field1"].value<int>() == 11);
		CHECK(table["field2"].value<int>() == 22);
		CHECK(table["foreground"].value<std::string>() == "Blue");

		CHECK(newdest.field1 == data.field1);
		CHECK(newdest.field2 == data.field2);
		CHECK(newdest.foreground == data.foreground);
	}
}

// clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
