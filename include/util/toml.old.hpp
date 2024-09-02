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
#include <any>
#include <initializer_list>
#include <string>
#include <type_traits>
#include <typeindex>
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

namespace TOML {
template<typename TClass>
struct AdlSerializer {
	static auto to_toml(const TClass& obj)
	{
		using class_t = std::decay_t<TClass>;
		return class_t::to_toml(obj);
	}
	static void from_toml(const toml::table& tbl, TClass& result)
	{
		using class_t = std::decay_t<TClass>;
		class_t::from_toml(tbl, result);
	}

	template<typename TField>
	static void
	insert_field(toml::table& tbl, const char* fieldName, const TField& obj)
	{
		using value_type = std::decay_t<TField>;

		if constexpr (is_toml_type_t<value_type>) {
			tbl.insert_or_assign(fieldName, obj);
		} else {
			if constexpr (std::is_enum_v<value_type>) {
				AdlSerializer::insert_enum_field<value_type>()
			} else {
			auto subtable = AdlSerializer<value_type>::to_toml(obj);
			tbl.insert_or_assign(fieldName, subtable);
		}
	}
	template<typename TField>
	static void insert_enum_field(
	    toml::table& tbl, const char* fieldName, TField obj
	);


	template<typename TField>
	static void extract_field(
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
			AdlSerializer<value_type>::from_toml(subtable, output);
		}
	}
};
}
}

#define IOCORE_TOML_FIELD(field)                                                \
	IOCore::TOML::AdlSerializer<decltype(obj                                \
	)>::insert_field<decltype(obj.field)>(tbl, #field, obj.field);
#define IOCORE_TOML_EXTRACT_FIELD(field)                                        \
	IOCore::TOML::AdlSerializer<decltype(result                             \
	)>::extract_field<decltype(result.field)>(tbl, #field, result.field);

#define IOCORE_TOML_SERIALIZABLE(CLASS, ...)                                    \
    private:                                                                    \
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
	friend IOCore::TOML::AdlSerializer<CLASS>;                              \
	;

#define IOCORE_TOML_ENUM(ENUM_TYPE, ...)                                        \
	template<typename T>                                                    \
	void IOCore::TOML::AdlSerializer<T>::insert_field(                      \
	    toml::table& tbl, const char* fieldName, ENUM_TYPE obj              \
	)                                                                       \
	{                                                                       \
		;                                                               \
	}                                                                       \
	template<typename T>                                                    \
	void IOCore::TOML::AdlSerializer<T>::extract_field(                     \
	    const toml::table& tbl, const char* fieldName, ENUM_TYPE& output    \
	)                                                                       \
	{                                                                       \
		;                                                               \
	}

#define IOCORE_TOML_SERIALIZE_IMPL(CLASS)                                       \
	template<>                                                              \
	auto AdlSerializer<CLASS>::to_table(const CLASS& obj)->toml::table

#define IOCORE_TOML_DESERIALIZE_IMPL(CLASS)                                     \
	template<>                                                              \
	void AdlSerializer<CLASS>::from_table(                                  \
	    const toml::table& tbl, CLASS& result                               \
	)

// clang-format off
// vim: set foldmethod=marker foldmarker=@{,@} foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
