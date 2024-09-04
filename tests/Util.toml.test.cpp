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
#include "test-utils/serialization.hpp"

#include "IOCore/util/toml.hpp"

BEGIN_TEST_SUITE("Util.Toml")
{
	TEST_CASE("SimpleStruct Serializes")
	{
		SimpleStruct data{ 1 };
		toml::table result;
		SimpleStruct deserialized;

		to_toml(result, data);
		from_toml(result, deserialized);

		REQUIRE(result.size() == 2);
		CHECK(result.at("field1").value<int>() == 1);
		CHECK(deserialized.field1 == 1);
		CHECK(deserialized.field2 == 0);
	}

	TEST_CASE("SimpleClass Serializes")
	{
		SimpleClass data{ 1, 22 };
		toml::table result;
		SimpleClass deserialized;

		to_toml(result, data);
		from_toml(result, deserialized);

		REQUIRE(result.size() == 2);
		CHECK(result.at("field_a").value<int>() == 1);
		CHECK(result.at("field_b").value<int>() == 22);

		CHECK(deserialized.field_a == 1);
		CHECK(deserialized.field_b == 22);
	}
	TEST_CASE("StructWithEnum Serializes")
	{
		StructWithEnum data{ 1, 2, Colors::Green };
		toml::table result;
		StructWithEnum deserialized;

		to_toml(result, data);
		from_toml(result, deserialized);

		REQUIRE(result.size() == 3);
		CHECK(result.at("field1").value<int>() == 1);
		CHECK(result.at("field2").value<int>() == 2);
		CHECK(result.at("foreground").value<std::string>() == "Green");

		CHECK(deserialized.field1 == data.field1);
		CHECK(deserialized.field2 == data.field2);
		CHECK(deserialized.foreground == data.foreground);
	}
	TEST_CASE("CompositeStruct Serializes")
	{
		CompositeStruct data{ { 1, 'c' }, { 3, 4 } };
		toml::table result;
		CompositeStruct deserialized;

		to_toml(result, data);
		from_toml(result, deserialized);

		REQUIRE(result.size() == 2);
		REQUIRE(result.at("part1").as_table()->size() == 2);
		REQUIRE(result.at("part2").as_table()->size() == 2);

		CHECK(deserialized.part1.field1 == data.part1.field1);
		CHECK(deserialized.part1.field2 == data.part1.field2);
		CHECK(deserialized.part2.field_a == data.part2.field_a);
		CHECK(deserialized.part2.field_b == data.part2.field_b);
	}

	TEST_CASE("ComplexStruct Serializes")
	{
		ComplexStruct data{
			{ 1, 'c' }, { 3, 4 }, { 5, 6, Colors::Red }, Colors::Blue
		};
		toml::table result;
		ComplexStruct deserialized;

		to_toml(result, data);
		from_toml(result, deserialized);

		REQUIRE(result.size() == 5);
		REQUIRE(result.at("part1").as_table()->size() == 2);
		REQUIRE(result.at("part2").as_table()->size() == 2);
		REQUIRE(result.at("part3").as_table()->size() == 3);

		CHECK(deserialized.part1.field1 == data.part1.field1);
		CHECK(deserialized.part1.field2 == data.part1.field2);
		CHECK(deserialized.part2.field_a == data.part2.field_a);
		CHECK(deserialized.part2.field_b == data.part2.field_b);
		CHECK(deserialized.part3.field1 == data.part3.field1);
		CHECK(deserialized.part3.field2 == data.part3.field2);
		CHECK(deserialized.part3.foreground == data.part3.foreground);
		CHECK(deserialized.background == data.background);
		CHECK(deserialized.mode == data.mode);
	}
}
// clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
