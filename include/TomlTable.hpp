/*
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

	template<typename T>
	auto operator=(const T& obj) -> TomlTable&
	{
		toml::table::operator=(to_toml_table(obj));
		return *this;
	}

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
};

}
// clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
