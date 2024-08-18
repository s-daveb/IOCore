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

template<typename T>
constexpr bool is_toml_type_t = toml::impl::can_partially_represent_native<T>;

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

namespace IOCore::TOML {

struct Serializer {
	template<typename TClass>
	static auto to_table(const TClass& obj) -> toml::table // NOLINT
	{
		using class_t = std::decay_t<TClass>;
		auto result = class_t::to_toml(obj);
		return result;
	}
	template<typename TClass>
	static void from_table(const toml::table& tbl, TClass& result) // NOLINT
	{
		using class_t = std::decay_t<TClass>;
		class_t::from_toml(tbl, result);
	}

	template<typename TField>
	static void insert_field( // NOLINT
	    toml::table& tbl, const char* fieldName, const TField& obj
	)
	{
		using value_type = std::decay_t<TField>;

		if constexpr (is_toml_type_t<value_type>) {
			tbl.insert_or_assign(fieldName, obj);
		} else {
			auto subtable = to_table(obj);
			tbl.insert_or_assign(fieldName, subtable);
		}
	}
	template<typename TField>
	static void extract_field( // NOLINT
	    const toml::table& tbl, const char* fieldName, TField& output
	)
	{
		using value_type = std::decay_t<TField>;

		if (!tbl.contains(fieldName)) {
			throw IOCore::TomlException(
			    "Missing field " + std::string(fieldName)
			);
		}
		if constexpr (is_toml_type_t<value_type>) {
			output = tbl[fieldName].value<value_type>().value();
		} else {
			auto subtable = *(tbl[fieldName].as_table());
			from_table(subtable, output);
		}
	}
};
}

#define IOCORE_TOML_FIELD(field)                                                \
	IOCore::TOML::Serializer::insert_field<decltype(obj.field)>(            \
	    tbl, #field, obj.field                                              \
	);
#define IOCORE_TOML_EXTRACT_FIELD(field)                                        \
	IOCore::TOML::Serializer::extract_field<decltype(result.field)>(        \
	    tbl, #field, result.field                                           \
	);

#define IOCORE_TOML_SERIALIZABLE(CLASS, ...)                                    \
	static constexpr auto _class_name()->const char*                        \
	{                                                                       \
		return #CLASS;                                                  \
	}                                                                       \
                                                                                \
	static auto to_toml(const CLASS& obj)->toml::table                      \
	{                                                                       \
		toml::table tbl;                                                \
		FOREACH_PARAM(IOCORE_TOML_FIELD, __VA_ARGS__)                   \
		return tbl;                                                     \
	}                                                                       \
                                                                                \
	static void from_toml(const toml::table& tbl, CLASS& result)            \
	{                                                                       \
		FOREACH_PARAM(IOCORE_TOML_EXTRACT_FIELD, __VA_ARGS__)           \
	}                                                                       \
	friend class IOCore::TOML::Serializer;

#define IOCORE_TOML_ENUM(ENUM_TYPE, ...)                                        \
	template<>                                                              \
	inline void IOCore::TOML::Serializer::insert_field<ENUM_TYPE>(          \
	    toml::table & tbl, const char* fieldName, const ENUM_TYPE& obj      \
	)                                                                       \
	{                                                                       \
		constexpr bool enum_check = std::is_enum<ENUM_TYPE>::value;     \
		static_assert(enum_check, #ENUM_TYPE "must be an enum!");       \
                                                                                \
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
	inline void IOCore::TOML::Serializer::extract_field<ENUM_TYPE>(         \
	    const toml::table& tbl, const char* fieldName, ENUM_TYPE& obj       \
	)                                                                       \
	{                                                                       \
		constexpr bool enum_check = std::is_enum<ENUM_TYPE>::value;     \
		static_assert(enum_check, #ENUM_TYPE "must be an enum!");       \
                                                                                \
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
