/* toml.hpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <algorithm>
#include <initializer_list>
#include <utility>

#include <toml++/toml.hpp>

#include "../Exception.hpp"
#include "./macros.hpp"

namespace IOCore::Toml {
struct TomlException;
struct Table;
}

template<typename T>
void add_to_toml_table(toml::table& tbl, const char* fieldName, const T& obj);
template<typename T>
void extract_from_toml_table(
    const toml::table& tbl, const char* fieldName, T& output
);

#define IOCORE_TOML_TO(field) add_to_toml_table(tbl, #field, obj.field);
#define IOCORE_TOML_FROM(field) extract_from_toml_table(tbl, #field, obj.field);

#define IOCORE_INIT_METADATA(T)                                                 \
	auto metadata = toml::table();                                          \
	metadata.insert_or_assign("type", #T);                                  \
	tbl.insert_or_assign("General", metadata);

#define IOCORE_TOML_SERIALIZABLE(T, ...)                                        \
	const char* _class_name = #T;                                           \
	friend void to_toml_table(toml::table& tbl, const T& obj)               \
	{                                                                       \
		IOCORE_FOREACH_PARAM(IOCORE_TOML_TO, __VA_ARGS__)               \
	}                                                                       \
                                                                                \
	friend void from_toml_table(const toml::table& tbl, T& obj)             \
	{                                                                       \
		IOCORE_FOREACH_PARAM(IOCORE_TOML_FROM, __VA_ARGS__)             \
	}

#define IOCORE_TOML_ENUM(ENUM_TYPE, ...)                                        \
	template<>                                                              \
	void add_to_toml_table<ENUM_TYPE>(                                      \
	    toml::table & tbl, const char* fieldName, const ENUM_TYPE& obj      \
	)                                                                       \
	{                                                                       \
		static_assert(                                                  \
		    std::is_enum<ENUM_TYPE>::value,                             \
		    #ENUM_TYPE " must be an enum!"                              \
		);                                                              \
		using pair_t = std::pair<ENUM_TYPE, const char*>;               \
		static const pair_t _enum_to_string[] = {                       \
			IOCORE_FOREACH_ENUM_PARAM(                              \
			    IOCORE_ENUM_FIELD, __VA_ARGS__                      \
			)                                                       \
		};                                                              \
		auto it = std::find_if(                                         \
		    std::begin(_enum_to_string),                                \
		    std::end(_enum_to_string),                                  \
		    [obj](const auto& pair) -> bool {                           \
			    return pair.first == obj;                           \
		    }                                                           \
		);                                                              \
		tbl.insert_or_assign(fieldName, it->second);                    \
	}                                                                       \
                                                                                \
	template<>                                                              \
	void extract_from_toml_table<ENUM_TYPE>(                                \
	    const toml::table& tbl, const char* fieldName, ENUM_TYPE& obj       \
	)                                                                       \
	{                                                                       \
		static_assert(                                                  \
		    std::is_enum<ENUM_TYPE>::value,                             \
		    #ENUM_TYPE " must be an enum!"                              \
		);                                                              \
		using pair_t = std::pair<ENUM_TYPE, const char*>;               \
		static const pair_t _enum_to_string[] = {                       \
			IOCORE_FOREACH_ENUM_PARAM(                              \
			    IOCORE_ENUM_FIELD, __VA_ARGS__                      \
			)                                                       \
		};                                                              \
		auto val = tbl[fieldName].value<std::string>().value();         \
		for (const auto& [enum_val, str] : _enum_to_string) {           \
			if (str == val) {                                       \
				obj = enum_val;                                 \
				break;                                          \
			}                                                       \
		}                                                               \
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

	template<typename T>
	auto as() -> T
	{
		T retval;
		from_toml_table(*this, retval);
		return retval;
	}
};
}

template<typename T>
void add_to_toml_table(toml::table& tbl, const char* fieldName, const T& obj)
{
	tbl.insert_or_assign(fieldName, obj);
}
template<typename T>
void extract_from_toml_table(
    const toml::table& tbl, const char* fieldName, T& output
)
{
	if (!tbl.contains(fieldName)) {
		throw IOCore::Toml::TomlException(
		    "Missing field " + std::string(fieldName)
		);
	}
	output = tbl[fieldName].value<T>().value();
}

// clang-format off
// vim: set foldmethod=marker foldmarker=@{,@} foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
