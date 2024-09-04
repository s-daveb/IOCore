/* test-util/serialization.hpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "IOCore/util/toml.hpp"

inline namespace util {
inline namespace serialization {

enum Colors { Red, Green, Blue };
TOML_ENUM(Colors, Red, Green, Blue);

namespace ns {
enum WindowMode { Windowed, Fullscreen, Borderless };
TOML_ENUM(WindowMode, Windowed, Fullscreen, Borderless);
} // namespace ns

struct SimpleStruct {
	int field1;
	char field2;

	auto operator==(const SimpleStruct& other) const -> bool
	{
		return field1 == other.field1 && field2 == other.field2;
	}
};
TOML_STRUCT(SimpleStruct, field1, field2);

class SimpleClass {
    public:
	int field1;
	int field2;

	auto operator==(const SimpleClass& other) const -> bool
	{
		return field1 == other.field1 && field2 == other.field2;
	}

    private:
	TOML_CLASS(SimpleClass, field1, field2);
};

struct StructWithEnum {
	int field1;
	int field2;
	Colors foreground;

	auto operator==(const StructWithEnum& other) const -> bool
	{
		return field1 == other.field1 && field2 == other.field2 &&
		       foreground == other.foreground;
	}
	TOML_CLASS(StructWithEnum, field1, field2, foreground);
};

struct CompositeStruct {
	SimpleStruct part1;
	SimpleClass part2;

	auto operator==(const CompositeStruct& other) const -> bool
	{
		return part1 == other.part1 && part2 == other.part2;
	}
	TOML_CLASS(CompositeStruct, part1, part2);
};

struct ComplexStruct {
	SimpleStruct part1;
	SimpleClass part2;
	StructWithEnum part3;
	Colors background;
	ns::WindowMode mode;

	TOML_CLASS(ComplexStruct, part1, part2, part3, background, mode);
};

} // namespace serialization
} // namespace util

// clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
