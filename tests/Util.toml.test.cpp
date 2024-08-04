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

BEGIN_TEST_SUITE("Util.Toml")
{
	struct SimpleStruct {
		int field1;
		int field2;

		IOCORE_TOML_SERIALIZABLE(SimpleStruct, field1, field2);
	};
	struct SimpleStruct2 {
		int fieldA;

		IOCORE_TOML_SERIALIZABLE(SimpleStruct2, fieldA);
	};

	struct StructWithEnum {
		int field1;
		int field2;
		Colors foreground;

		IOCORE_TOML_SERIALIZABLE(
		    StructWithEnum, field1, field2, foreground
		);
	};

	struct CompositeStruct {
		SimpleStruct part1;
		SimpleStruct2 part2;

		IOCORE_TOML_SERIALIZABLE(CompositeStruct, part1, part2);
	};

	struct ComplexStruct {
		SimpleStruct part1;
		SimpleStruct2 part2;
		StructWithEnum part3;
		Colors background;

		IOCORE_TOML_SERIALIZABLE(
		    ComplexStruct, part1, part2, part3, background
		);
	};
	TEST_CASE("IOCORE_TOML_TO Macro works")
	{
		toml::table tbl;
		SimpleStruct obj;

		IOCORE_TOML_TO(field1);
		IOCORE_TOML_TO(field2);
		REQUIRE(tbl.size() == 2);
	}
	TEST_CASE("IOCORE_TOML_SERIALIZABLE Macro works with SimpleStruct")
	{
		toml::table table;
		SimpleStruct data = { 10, 20 };
		SimpleStruct newdest;

		table = to_toml_table(data);
		from_toml_table(table, newdest);

		CHECK(table.size() == 2);

		CHECK(table["field1"].value<int>() == 10);
		CHECK(table["field2"].value<int>() == 20);

		CHECK(newdest.field1 == data.field1);
		CHECK(newdest.field2 == data.field2);
	}
	TEST_CASE("IOCORE_TOML_SERIALIZABLE Macro works with StructWithEnum")
	{
		toml::table table;
		StructWithEnum data = { 11, 22, Green };
		StructWithEnum newdest;

		table = to_toml_table(data);
		from_toml_table(table, newdest);

		CHECK(table.size() == 3);

		CHECK(table["field1"].value<int>() == 11);
		CHECK(table["field2"].value<int>() == 22);
		CHECK(table["foreground"].value<std::string>() == "Green");

		CHECK(newdest.field1 == data.field1);
		CHECK(newdest.field2 == data.field2);
		CHECK(newdest.foreground == data.foreground);
	}

	TEST_CASE("IOCORE_TOML_SERIALIZABLE Macro works with CompositeStruct")
	{
		toml::table table;
		CompositeStruct data = { { 10, 20 }, { 30 } };
		CompositeStruct newdest;

		table = to_toml_table(data);
		from_toml_table(table, newdest);

		CHECK(table.size() == 2);

		CHECK(table["part1"].as_table()->size() == 2);
		CHECK(table["part2"].as_table()->size() == 1);

		CHECK(table["part1"]["field1"].value<int>() == 10);
		CHECK(table["part1"]["field2"].value<int>() == 20);
		CHECK(table["part2"]["fieldA"].value<int>() == 30);

		CHECK(newdest.part1.field1 == data.part1.field1);
		CHECK(newdest.part1.field2 == data.part1.field2);
		CHECK(newdest.part2.fieldA == data.part2.fieldA);
	}
	TEST_CASE("IOCORE_TOML_SERIALIZABLE Macro works with ComplexStruct")
	{
		toml::table table;
		ComplexStruct data = {
			{ 10, 20 }, { 30 }, { 40, 50, Blue }, Red
		};
		ComplexStruct newdest;

		table = to_toml_table(data);
		from_toml_table(table, newdest);

		CHECK(table.size() == 4);

		CHECK(table["part1"].as_table()->size() == 2);
		CHECK(table["part2"].as_table()->size() == 1);
		CHECK(table["part3"].as_table()->size() == 3);

		CHECK(table["part1"]["field1"].value<int>() == 10);
		CHECK(table["part1"]["field2"].value<int>() == 20);
		CHECK(table["part2"]["fieldA"].value<int>() == 30);
		CHECK(table["part3"]["field1"].value<int>() == 40);
		CHECK(table["part3"]["field2"].value<int>() == 50);
		CHECK(
		    table["part3"]["foreground"].value<std::string>() == "Blue"
		);

		CHECK(newdest.part1.field1 == data.part1.field1);
		CHECK(newdest.part1.field2 == data.part1.field2);
		CHECK(newdest.part2.fieldA == data.part2.fieldA);
		CHECK(newdest.part3.field1 == data.part3.field1);
		CHECK(newdest.part3.field2 == data.part3.field2);
		CHECK(newdest.part3.foreground == data.part3.foreground);
		CHECK(newdest.background == data.background);
	}
}
// clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
