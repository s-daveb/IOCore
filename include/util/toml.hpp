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
#include <type_traits>
#include <utility>

#include <toml++/toml.hpp>

#include "../Exception.hpp"
#include "./macros.hpp"

/* NOLINTBEGIN */
// Primary template, defaults to false
template<typename T>
struct is_toml_type : std::false_type {};

// Specializations for TOML++ native types
template<>
struct is_toml_type<double> : std::true_type {};

template<>
struct is_toml_type<int64_t> : std::true_type {};

template<>
struct is_toml_type<bool> : std::true_type {};

template<>
struct is_toml_type<std::string> : std::true_type {};

template<>
struct is_toml_type<std::string_view> : std::true_type {};

template<typename T>
constexpr bool is_not_toml_native_t = is_toml_type<T>::value;
/* NOLINTEND */

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
void insert_element(toml::table& tbl, const char* fieldName, const T& obj);
template<typename T>
void extract_element(const toml::table& tbl, const char* fieldName, T& output);

template<typename T>
auto to_toml_table(const T& obj) -> toml::table
{
	auto result = to_toml_table_impl(obj);
	return result;
}
template<typename T>
void from_toml_table(const toml::table& tbl, T& result)
{
	from_toml_table_impl(tbl, result);
}

#define IOCORE_TOML_TO(field) insert_element(tbl, #field, obj.field);
#define IOCORE_TOML_FROM(field) extract_element(tbl, #field, result.field);

#define IOCORE_TOML_SERIALIZABLE(CLASS, ...)                                    \
	static constexpr auto _class_name()->const char*                        \
	{                                                                       \
		return #CLASS;                                                  \
	}                                                                       \
                                                                                \
	friend auto to_toml_table_impl(const CLASS& obj)->toml::table           \
	{                                                                       \
		toml::table tbl;                                                \
		FOREACH_PARAM(IOCORE_TOML_TO, __VA_ARGS__)                      \
		return tbl;                                                     \
	}                                                                       \
                                                                                \
	friend void from_toml_table_impl(const toml::table& tbl, CLASS& result) \
	{                                                                       \
		FOREACH_PARAM(IOCORE_TOML_FROM, __VA_ARGS__)                    \
	}

template<typename T>
inline void insert_element(toml::table& tbl, const char* fieldName, const T& obj)
{
	using value_type = std::decay_t<T>;

	if constexpr (is_not_toml_native_t<value_type> || std::is_class_v<value_type>) {
		auto subtable = to_toml_table(obj);
		tbl.insert_or_assign(fieldName, subtable);
	} else {

		tbl.insert_or_assign(fieldName, obj);
	}
}
template<typename T>
inline void
extract_element(const toml::table& tbl, const char* fieldName, T& output)
{
	using value_type = std::decay_t<T>;

	if (!tbl.contains(fieldName)) {
		throw IOCore::TomlException(
		    "Missing field " + std::string(fieldName)
		);
	}
	if constexpr (is_not_toml_native_t<value_type> || std::is_class_v<value_type>) {
		auto subtable = *(tbl[fieldName].as_table());
		from_toml_table(subtable, output);
	} else {
		output = tbl[fieldName].value<T>().value();
	}
}

#define IOCORE_TOML_ENUM(ENUM_TYPE, ...)                                        \
	template<>                                                              \
	inline void insert_element<ENUM_TYPE>(                                  \
	    toml::table & tbl, const char* fieldName, const ENUM_TYPE& obj      \
	)                                                                       \
	{                                                                       \
		static_assert(                                                  \
		    std::is_enum<ENUM_TYPE>::value,                             \
		    #ENUM_TYPE "must be an enum!"                               \
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
	inline void extract_element<ENUM_TYPE>(                                 \
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

// clang-format off
// vim: set foldmethod=marker foldmarker=@{,@} foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
