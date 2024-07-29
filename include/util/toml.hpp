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
#include <string>
#include <utility>

#include <toml++/toml.hpp>

#include "../Exception.hpp"
#include "./macros.hpp"

namespace IOCore {
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
}

template<typename T>
void add_to_toml_table(toml::table& tbl, const char* fieldName, const T& obj);
template<typename T>
void extract_from_toml_table(
    const toml::table& tbl, const char* fieldName, T& output
);

#define IOCORE_TOML_TO(field) add_to_toml_table(tbl, #field, obj.field);
#define IOCORE_TOML_FROM(field) extract_from_toml_table(tbl, #field, obj.field);

#define TOML_INIT_METADATA(T)                                                   \
	auto metadata = toml::table();                                          \
	metadata.insert_or_assign("type", #T);                                  \
	tbl.insert_or_assign("General", metadata);

#define IOCORE_TOML_SERIALIZABLE(T, ...)                                        \
	const char* _class_name = #T;                                           \
	friend void to_toml_table(toml::table& tbl, const T& obj)               \
	{                                                                       \
		FOREACH_PARAM(IOCORE_TOML_TO, __VA_ARGS__)                      \
	}                                                                       \
                                                                                \
	friend void from_toml_table(const toml::table& tbl, T& obj)             \
	{                                                                       \
		FOREACH_PARAM(IOCORE_TOML_FROM, __VA_ARGS__)                    \
	}

#define IOCORE_TOML_ENUM(ENUM_TYPE, ...)                                        \
	template<>                                                              \
	inline void add_to_toml_table<ENUM_TYPE>(                               \
	    toml::table & tbl, const char* fieldName, const ENUM_TYPE& obj      \
	)                                                                       \
	{                                                                       \
		static_assert(                                                  \
		    std::is_enum<ENUM_TYPE>::value,                             \
		    #ENUM_TYPE " must be an enum!"                              \
		);                                                              \
		using pair_t = std::pair<ENUM_TYPE, const char*>;               \
		static const pair_t _enum_to_string[] = {                       \
			FOREACH_ENUM_PARAM(TOML_ENUM_FIELD, __VA_ARGS__)        \
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
	inline void extract_from_toml_table<ENUM_TYPE>(                         \
	    const toml::table& tbl, const char* fieldName, ENUM_TYPE& obj       \
	)                                                                       \
	{                                                                       \
		static_assert(                                                  \
		    std::is_enum<ENUM_TYPE>::value,                             \
		    #ENUM_TYPE " must be an enum!"                              \
		);                                                              \
		using pair_t = std::pair<ENUM_TYPE, const char*>;               \
		static const pair_t _enum_to_string[] = {                       \
			FOREACH_ENUM_PARAM(TOML_ENUM_FIELD, __VA_ARGS__)        \
		};                                                              \
		auto val = tbl[fieldName].value<std::string>().value();         \
		for (const auto& [enum_val, str] : _enum_to_string) {           \
			if (str == val) {                                       \
				obj = enum_val;                                 \
				break;                                          \
			}                                                       \
		}                                                               \
	}

template<typename T>
inline void
add_to_toml_table(toml::table& tbl, const char* fieldName, const T& obj)
{
	tbl.insert_or_assign(fieldName, obj);
}
template<typename T>
inline void
extract_from_toml_table(const toml::table& tbl, const char* fieldName, T& output)
{
	if (!tbl.contains(fieldName)) {
		throw IOCore::TomlException(
		    "Missing field " + std::string(fieldName)
		);
	}
	output = tbl[fieldName].value<T>().value();
}

// clang-format off
// vim: set foldmethod=marker foldmarker=@{,@} foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
