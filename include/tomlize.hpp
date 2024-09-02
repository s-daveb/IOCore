/* tomlize.hpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v2.0 (MPL2)
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <toml++/toml.hpp>

namespace TOML {

using ErrorFlag = bool;
static constexpr ErrorFlag error_t = true;
static constexpr ErrorFlag success_t = false;

template<typename T>
auto to_toml(const T&) -> toml::table;

template<typename T>
auto from_toml(const toml::table&, T&) -> ErrorFlag;

template<typename TField>
void add_toml_field(const TField&, toml::table&);

template<typename TField>
auto extract_toml_field(const toml::table&, TField&) -> ErrorFlag;

#define TOMILIZE_FIELD(FIELD) add_toml_field(FIELD, table);
#define DETOMLIZE_FIELD(FIELD) result |= extract_toml_field(table, FIELD);

#define TOMLIZE_STRUCT(STRUCT, ...)                                             \
	template<>                                                              \
	auto to_toml(const STRUCT& s)->toml::table                              \
	{                                                                       \
		toml::table table;                                              \
		FOREACH_PARAM(TOMLIZE_FIELD, __VA_ARGS__);                      \
		return table;                                                   \
	}                                                                       \
	template<>                                                              \
	auto from_toml(const toml::table& table, STRUCT& s)->ErrorFlag          \
	{                                                                       \
		auto result = success_t;                                        \
		FOREACH_PARAM(DETOMLIZE_FIELD, __VA_ARGS__);                    \
		return result;                                                  \
	}

} // namespace TOML
  // clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
