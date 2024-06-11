/* toml.hpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <initializer_list>
#include <utility>

#include <toml++/toml.hpp>

#include "../Exception.hpp"
#include "./macros.hpp"

#define IOCORE_TOML_TO(field) tbl.insert_or_assign(#field, obj.field);
#define IOCORE_TOML_FROM(field)                                                 \
	if (!tbl.contains(#field)) {                                            \
		throw IOCore::Toml::TomlException("Missing field " #field);     \
	}                                                                       \
                                                                                \
	obj.field = tbl[#field].value<decltype(obj.field)>().value();

#define IOCORE_INIT_METADATA(T)                                                 \
	auto metadata = toml::table();                                          \
	metadata.insert_or_assign("type", #T);                                  \
	tbl.insert_or_assign("General", metadata);

#define IOCORE_TOML_SERIALIZABLE(T, ...)                                        \
	const char* _class_name = #T;                                           \
	friend void to_toml_table(toml::table& tbl, const T& obj)               \
	{                                                                       \
		IOCORE_INIT_METADATA(T)                                         \
		IOCORE_FOREACH(IOCORE_TOML_TO, __VA_ARGS__)                     \
	}                                                                       \
	friend void from_toml_table(const toml::table& table, T& obj)           \
	{                                                                       \
		auto tbl = table;                                               \
		tbl.erase("General");                                           \
		IOCORE_FOREACH(IOCORE_TOML_FROM, __VA_ARGS__)                   \
	}

namespace IOCore::Toml {

struct TomlException : public Exception {

	TomlException()
	    : Exception("TOML Serialization/Deseralization Exception")
	{
	}
	explicit TomlException(const std::string& message) : Exception(message)
	{
	}
	~TomlException() override = default;
};

struct Table : public toml::table {
	Table() = default;
	Table(const toml::table& tbl) : toml::table(tbl) {}
	Table(const Table& tbl) : toml::table(tbl) {}
	Table(Table&& tbl) noexcept : toml::table(std::move(tbl)) {}

	template<typename T>
	Table(const T& obj)
	{
		to_toml_table(*this, obj);
	}

	template<typename T>
	auto operator=(const T& obj) -> Table&
	{
		to_toml_table(*this, obj);
		return *this;
	}

	template<typename T>
	auto operator=(T&& obj) -> Table&
	{
		to_toml_table(*this, obj);
		return *this;
	}
};
}

// clang-format off
// vim: set foldmethod=marker foldmarker=@{,@} foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
