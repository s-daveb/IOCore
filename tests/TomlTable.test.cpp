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

namespace impl {
enum Colors { Red, Green, Blue };
IOCORE_TOML_ENUM(Colors, Red, Green, Blue);
}

BEGIN_TEST_SUITE("IOCore::TomlTable")
{
	using IOCore::TomlTable;
	struct SimpleStruct {
		int field1;
		int field2;

		auto operator==(const SimpleStruct& other) const -> bool
		{
			return field1 == other.field1 && field2 == other.field2;
		}

		IOCORE_TOML_SERIALIZABLE(SimpleStruct, field1, field2);
	};
	struct SimpleStruct2 {
		int field1;
		int field2;
		std::string label;

		auto operator==(const SimpleStruct2& other) const -> bool
		{
			return label == other.label && field1 == other.field1 &&
			       field2 == other.field2;
		}

		IOCORE_TOML_SERIALIZABLE(SimpleStruct2, field1, field2, label);
	};

	struct ComplexStruct {
		SimpleStruct field1;
		int field2;
		Colors foreground;

		IOCORE_TOML_SERIALIZABLE(
		    ComplexStruct, field1, field2, foreground
		);
	};

	TEST_CASE("IOCore::TomlTable class construction")
	{
		TomlTable table;
	}
	TEST_CASE("IOCore::TomlTable core operators, simple struct")
	{
		auto data = SimpleStruct2{ 11, 22, "Hello" };
		TomlTable table = data;

		CHECK(table.size() == 3);

		CHECK(table["field1"].value<int>() == 11);
		CHECK(table["field2"].value<int>() == 22);
		REQUIRE(table["label"].value<std::string>() == "Hello");
	}
	TEST_CASE("IOCore::TomlTable core operators complex struct")
	{
		auto data = ComplexStruct{ { 11, 22 }, 30, Blue };
		TomlTable table = data;
		auto newdest = table.as<ComplexStruct>();

		CHECK(table.size() == 3);

		CHECK(table["field1"]["field1"].value<int>() == 11);
		CHECK(table["field1"]["field2"].value<int>() == 22);
		CHECK(table["field2"].value<int>() == 30);
		CHECK(table["foreground"].value<std::string>() == "Blue");

		CHECK(newdest.field1 == data.field1);
		CHECK(newdest.field2 == data.field2);
		CHECK(newdest.foreground == data.foreground);
	}
}

// clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
