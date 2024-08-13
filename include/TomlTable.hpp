/* TomlTable.hpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v2.0 (MPL2)
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "util/toml.hpp"

#include <toml++/toml.hpp>

namespace IOCore {

struct TomlTable : public toml::table {
	TomlTable() = default;
	TomlTable(const toml::table& tbl) : toml::table(tbl) {}
	TomlTable(const TomlTable& tbl) : toml::table(tbl) {}
	TomlTable(TomlTable&& tbl) noexcept : toml::table(std::move(tbl)) {}

	template<typename T>
	TomlTable(const T& obj) : toml::table(to_toml_table(obj))
	{
	}

	// Copy assignment
	template<typename T>
	auto operator=(const T& obj) -> TomlTable&
	{
		toml::table::operator=(to_toml_table<T>(obj));
		return *this;
	}

	// Move assignment
	template<typename T>
	auto operator=(T&& obj) -> TomlTable&
	{
		toml::table::operator=(std::move(to_toml_table(obj)));
		return *this;
	}

	template<typename T>
	auto as() -> T
	{
		T retval;
		from_toml_table(*this, retval);
		return retval;
	}

	template<typename T>
	inline auto get() -> T
	{
		return this->as<T>();
	}
};

// @{ Template specializattions, overrides, and stream operators
// @{ Template specializations for toml::table
// Copy assignment template override for toml::table
template<>
inline auto TomlTable::operator=<toml::table>(const toml::table& tbl)
    -> TomlTable&
{
	toml::table::operator=(tbl);
	return *this;
}
// Move assignment template override for toml::table
template<>
inline auto TomlTable::operator=<toml::table>(toml::table&& tbl) -> TomlTable&
{
	toml::table::operator=(std::move(tbl));
	return *this;
} // @}
// @{ Stream operator wrappers (around toml::table)
inline auto operator<<(std::ostream& output_stream, const TomlTable& table)
    -> std::ostream&
{
	output_stream << static_cast<toml::table>(table);
	return output_stream;
}

inline auto operator>>(const std::istream& input_stream, TomlTable& table)
    -> const std::istream&
{
	std::stringstream buffer;
	buffer << input_stream.rdbuf();
	table = toml::parse(buffer.str());
	return input_stream;
} // @}
} // @}

// clang-format off
// vim: set foldmethod=marker foldmarker=@{,@} foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
