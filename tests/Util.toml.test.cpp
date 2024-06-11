/* Util.toml.test.cpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "test-utils/common.hpp"

#include "../include/util/toml.hpp"

BEGIN_TEST_SUITE("Util.Toml")
{
	struct TestStruct {
		int field1;
		int field2;

		friend void
		to_toml_table(toml::table& tbl, const TestStruct& obj)
		{
			IOCORE_TOML_TO(field1);
			IOCORE_TOML_TO(field2);
		}
	};

	struct SerializableStruct {
		int field1;
		int field2;

		IOCORE_TOML_SERIALIZABLE(SerializableStruct, field1, field2);
	};

	TEST_CASE("IOCORE_TOML_TO Macro works")
	{

		toml::table table;
		TestStruct data;

		to_toml_table(table, data);
		REQUIRE(table.size() == 2);
	}
	TEST_CASE("IOCORE_TOML_SERIALIZABLE Macro works")
	{

		toml::table table;
		SerializableStruct data = { 11, 22 };
		SerializableStruct newdest;

		to_toml_table(table, data);
		from_toml_table(table, newdest);

		REQUIRE(table.size() == 3);

		REQUIRE(table["field1"].value<int>() == 11);
		REQUIRE(table["field2"].value<int>() == 22);

		REQUIRE(newdest.field1 == data.field1);
		REQUIRE(newdest.field2 == data.field2);
	}
	TEST_CASE("Toml::Table class construction and basic operators")
	{
		IOCore::Toml::Table table = SerializableStruct{ 11, 22 };

		REQUIRE(table.size() == 3);
		REQUIRE(
		    table["General"]["type"].value<std::string>().value() ==
		    "SerializableStruct"
		);
	}
}

// clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
