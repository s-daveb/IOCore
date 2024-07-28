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

#include "../include/util/toml.hpp"

enum Colors { Red, Green, Blue };

IOCORE_TOML_ENUM(Colors, Red, Green, Blue);
//  IOCORE_TOML_ENUM(Colors, { Red, "Red" }, { Green, "Green" }, { Blue, "Blue"
//  });

BEGIN_TEST_SUITE("Util.Toml")
{
	struct SimpleStruct {
		int field1;
		int field2;

		// friend void
		// to_toml_table(toml::table& tbl, const SimpleStruct& obj)
		//{
		//	IOCORE_TOML_TO(field1);
		//	IOCORE_TOML_TO(field2);
		// }

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

	TEST_CASE("IOCORE_TOML_TO Macro works")
	{

		toml::table table;
		SimpleStruct data;

		to_toml_table(table, data);
		REQUIRE(table.size() == 2);
	}
	TEST_CASE("IOCORE_TOML_SERIALIZABLE Macro works")
	{

		toml::table table;
		ComplexStruct data = { 11, 22, Green };
		ComplexStruct newdest;

		to_toml_table(table, data);
		from_toml_table(table, newdest);

		REQUIRE(table.size() == 3);

		REQUIRE(table["field1"].value<int>() == 11);
		REQUIRE(table["field2"].value<int>() == 22);
		REQUIRE(table["foreground"].value<std::string>() == "Green");

		REQUIRE(newdest.field1 == data.field1);
		REQUIRE(newdest.field2 == data.field2);
		REQUIRE(newdest.foreground == data.foreground);
	}
	TEST_CASE("Toml::Table class construction and basic operators")
	{
		IOCore::Toml::Table table = ComplexStruct{ 11, 22 };

		REQUIRE(table.size() == 3);
		// REQUIRE(
		//     table["General"]["type"].value<std::string>().value() ==
		//     "SerializableStruct"
		//);
	}
}

// clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
