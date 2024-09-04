/* util/toml.hpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v2.0 (MPL2)
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <toml++/toml.hpp>

#include "../Exception.hpp"
#include "macros.hpp"

inline namespace TOML {

struct TomlException : public IOCore::Exception {

	TomlException()
	    : IOCore::Exception("TOML Serialization/Deseralization Exception")
	{
	}
	explicit TomlException(const std::string& message)
	    : IOCore::Exception(message)
	{
	}
	~TomlException() override = default;
};

template<typename T>
constexpr bool is_toml_type_t = toml::impl::can_partially_represent_native<T>;

template<typename T>
void to_toml(toml::table&, const T&);

template<typename T>
void from_toml(const toml::table&, T&);

template<typename T>
auto create_toml(const T& obj) -> toml::table
{
	toml::table tbl;
	to_toml(tbl, obj);
	return tbl;
}

template<typename TField>
inline void
add_toml_field(const TField& obj, const char* fieldName, toml::table& tbl)
{
	using value_type = std::decay_t<TField>;

	if constexpr (std::is_enum_v<value_type>) {
		add_toml_enum_field(obj, fieldName, tbl);
	} else if constexpr (std::is_same_v<char, value_type>) {
		tbl.insert_or_assign(fieldName, static_cast<int>(obj));
		return;
	} else {
		if constexpr (is_toml_type_t<value_type>) {
			tbl.insert_or_assign(fieldName, obj);
		} else {
			toml::table subtable;
			to_toml(subtable, obj);
			tbl.insert_or_assign(fieldName, subtable);
		}
	}
}
template<typename TField>
inline void
extract_toml_field(const toml::table& tbl, const char* fieldName, TField& output)
{
	using value_type = std::decay_t<TField>;

	if (!tbl.contains(fieldName)) {
		throw TomlException("Missing field " + std::string(fieldName));
	}

	if constexpr (std::is_enum_v<value_type>) {
		extract_toml_enum_field(tbl, fieldName, output);
	} else if constexpr (std::is_same_v<char, value_type>) {
		output = tbl[fieldName].value<int>().value();
	} else if constexpr (is_toml_type_t<value_type>) {
		output = tbl[fieldName].value<value_type>().value();
	} else {
		auto subtable = *(tbl[fieldName].as_table());
		from_toml(subtable, output);
	}
}

} // namespace TOML

#define INSERT_FIELD(FIELD) add_toml_field(obj.FIELD, #FIELD, table);
#define EXTRACT_FIELD(FIELD) extract_toml_field(table, #FIELD, obj.FIELD);

#define ENUM_FIELD_ENTRY(field) NAMED_PAIR(field),

#define TOML_STRUCT(STRUCT_TYPE, ...)                                           \
	inline void to_toml(toml::table& table, const STRUCT_TYPE& obj)         \
	{                                                                       \
		FOREACH_PARAM(INSERT_FIELD, __VA_ARGS__);                       \
	}                                                                       \
                                                                                \
	inline void from_toml(const toml::table& table, STRUCT_TYPE& obj)       \
	{                                                                       \
		FOREACH_PARAM(EXTRACT_FIELD, __VA_ARGS__);                      \
	}

#define TOML_CLASS(CLASS_TYPE, ...)                                             \
	inline friend void to_toml(toml::table& table, const CLASS_TYPE& obj)   \
	{                                                                       \
		FOREACH_PARAM(INSERT_FIELD, __VA_ARGS__);                       \
	}                                                                       \
                                                                                \
	inline friend void from_toml(const toml::table& table, CLASS_TYPE& obj) \
	{                                                                       \
		FOREACH_PARAM(EXTRACT_FIELD, __VA_ARGS__);                      \
	}

#define TOML_ENUM(ENUM_TYPE, ...)                                               \
	inline void add_toml_enum_field(                                        \
	    const ENUM_TYPE& obj, const char* fieldName, toml::table& tbl       \
	)                                                                       \
	{                                                                       \
		static_assert(                                                  \
		    std::is_enum<ENUM_TYPE>::value,                             \
		    #ENUM_TYPE "must be an enum!"                               \
		);                                                              \
		using pair_t = std::pair<const char*, ENUM_TYPE>;               \
		static const pair_t _enum_to_string[] = {                       \
			FOREACH_PARAM(ENUM_FIELD_ENTRY, __VA_ARGS__)            \
		};                                                              \
		auto it = std::find_if(                                         \
		    std::begin(_enum_to_string),                                \
		    std::end(_enum_to_string),                                  \
		    [obj](const auto& pair) -> bool {                           \
			    return pair.second == obj;                          \
		    }                                                           \
		);                                                              \
		tbl.insert_or_assign(fieldName, it->first);                     \
	}                                                                       \
	inline void extract_toml_enum_field(                                    \
	    const toml::table& tbl, const char* fieldName, ENUM_TYPE& obj       \
	)                                                                       \
	{                                                                       \
		static_assert(                                                  \
		    std::is_enum<ENUM_TYPE>::value,                             \
		    #ENUM_TYPE " must be an enum!"                              \
		);                                                              \
		using pair_t = std::pair<const char*, ENUM_TYPE>;               \
		static const pair_t _enum_to_string[] = {                       \
			FOREACH_PARAM(ENUM_FIELD_ENTRY, __VA_ARGS__)            \
		};                                                              \
		auto val = tbl[fieldName].value<std::string>().value();         \
		for (const auto& [str, enum_val] : _enum_to_string) {           \
			if (str == val) {                                       \
				obj = enum_val;                                 \
				break;                                          \
			}                                                       \
		}                                                               \
	}

#define TOML_SERIALIZE_IMPL(CLASS)                                              \
	template<>                                                              \
	void to_toml<CLASS>(toml::table & result, const CLASS& obj)
#define TOML_DESERIALIZE_IMPL(CLASS)                                            \
	template<>                                                              \
	void from_toml<CLASS>(const toml::table& tbl, CLASS& result)

// clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
