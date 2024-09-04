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

#include "IOCore/TomlTable.hpp"
#include "IOCore/util/toml.hpp"

#include "test-utils/serialization.hpp"

BEGIN_TEST_SUITE("IOCore::TomlTable")
{
	using IOCore::TomlTable;
	TEST_CASE("IOCore::TomlTable class construction")
	{
		TomlTable table;
	}
	TEST_CASE("IOCore::TomlTable core operators, simple struct")
	{
		auto data = SimpleClass{ 11, 22 };
		TomlTable table = data;

		CHECK(table.size() == 2);

		CHECK(table["field1"].value<int>() == 11);
		CHECK(table["field2"].value<int>() == 22);
	}
	TEST_CASE("IOCore::TomlTable core operators complex struct")
	{
		auto data = ComplexStruct{};
		data.part1 = { 1, 'a' };
		data.part2 = { 3, 4 };
		data.part3 = { 5, 6, Blue };
		data.background = Red;
		data.mode = ns::Windowed;

		TomlTable table = data;
		auto newdest = table.get<ComplexStruct>();

		REQUIRE(table.size() == 5);

		CHECK(table["part1"]["field1"].value<int>() == 1);
		CHECK(table["part1"]["field2"].value<int>() == 'a');
		CHECK(table["part2"]["field1"].value<int>() == 3);
		CHECK(table["part2"]["field2"].value<int>() == 4);
		CHECK(table["part3"]["field1"].value<int>() == 5);
		CHECK(table["part3"]["field2"].value<int>() == 6);

		CHECK(
		    table["part3"]["foreground"].value<std::string>() == "Blue"
		);
		CHECK(table["background"].value<std::string>() == "Red");
		CHECK(table["mode"].value<std::string>() == "Windowed");
	}
}

// clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
