/*
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */
// clang-format off
//
#pragma once

#define TOML_EXPAND(x) x
#define TOML_ENUM_FIELD(field) { field, #field }

#define FOREACH_PARAM_1(func, param) func(param)
#define FOREACH_PARAM_2(func, param, ...) func(param); TOML_EXPAND(FOREACH_PARAM_1(func, __VA_ARGS__))
#define FOREACH_PARAM_3(func, param, ...) func(param); TOML_EXPAND(FOREACH_PARAM_2(func, __VA_ARGS__))
#define FOREACH_PARAM_4(func, param, ...) func(param); TOML_EXPAND(FOREACH_PARAM_3(func, __VA_ARGS__))
#define FOREACH_PARAM_5(func, param, ...) func(param); TOML_EXPAND(FOREACH_PARAM_4(func, __VA_ARGS__))
#define FOREACH_PARAM_6(func, param, ...) func(param); TOML_EXPAND(FOREACH_PARAM_5(func, __VA_ARGS__))
#define FOREACH_PARAM_7(func, param, ...) func(param); TOML_EXPAND(FOREACH_PARAM_6(func, __VA_ARGS__))
#define FOREACH_PARAM_8(func, param, ...) func(param); TOML_EXPAND(FOREACH_PARAM_7(func, __VA_ARGS__))
#define FOREACH_PARAM_9(func, param, ...) func(param); TOML_EXPAND(FOREACH_PARAM_8(func, __VA_ARGS__))
#define FOREACH_PARAM_10(func, param, ...) func(param); TOML_EXPAND(FOREACH_PARAM_9(func, __VA_ARGS__))
#define FOREACH_PARAM_11(func, param, ...) func(param); TOML_EXPAND(FOREACH_PARAM_10(func, __VA_ARGS__))
#define FOREACH_PARAM_12(func, param, ...) func(param); TOML_EXPAND(FOREACH_PARAM_11(func, __VA_ARGS__))
#define FOREACH_PARAM_13(func, param, ...) func(param); TOML_EXPAND(FOREACH_PARAM_12(func, __VA_ARGS__))
#define FOREACH_PARAM_14(func, param, ...) func(param); TOML_EXPAND(FOREACH_PARAM_13(func, __VA_ARGS__))
#define FOREACH_PARAM_15(func, param, ...) func(param); TOML_EXPAND(FOREACH_PARAM_14(func, __VA_ARGS__))
#define FOREACH_PARAM_16(func, param, ...) func(param); TOML_EXPAND(FOREACH_PARAM_15(func, __VA_ARGS__))
#define FOREACH_PARAM_17(func, param, ...) func(param); TOML_EXPAND(FOREACH_PARAM_16(func, __VA_ARGS__))
#define FOREACH_PARAM_18(func, param, ...) func(param); TOML_EXPAND(FOREACH_PARAM_17(func, __VA_ARGS__))
#define FOREACH_PARAM_19(func, param, ...) func(param); TOML_EXPAND(FOREACH_PARAM_18(func, __VA_ARGS__))
#define FOREACH_PARAM_20(func, param, ...) func(param); TOML_EXPAND(FOREACH_PARAM_19(func, __VA_ARGS__))
#define FOREACH_PARAM_21(func, param, ...) func(param); TOML_EXPAND(FOREACH_PARAM_20(func, __VA_ARGS__))


#define FOREACH_ENUM_PARAM_1(func, param) func(param)
#define FOREACH_ENUM_PARAM_2(func, param, ...) func(param), TOML_EXPAND(FOREACH_ENUM_PARAM_1(func, __VA_ARGS__))
#define FOREACH_ENUM_PARAM_3(func, param, ...) func(param), TOML_EXPAND(FOREACH_ENUM_PARAM_2(func, __VA_ARGS__))
#define FOREACH_ENUM_PARAM_4(func, param, ...) func(param), TOML_EXPAND(FOREACH_ENUM_PARAM_3(func, __VA_ARGS__))
#define FOREACH_ENUM_PARAM_5(func, param, ...) func(param), TOML_EXPAND(FOREACH_ENUM_PARAM_4(func, __VA_ARGS__))
#define FOREACH_ENUM_PARAM_6(func, param, ...) func(param), TOML_EXPAND(FOREACH_ENUM_PARAM_5(func, __VA_ARGS__))
#define FOREACH_ENUM_PARAM_7(func, param, ...) func(param), TOML_EXPAND(FOREACH_ENUM_PARAM_6(func, __VA_ARGS__))
#define FOREACH_ENUM_PARAM_8(func, param, ...) func(param), TOML_EXPAND(FOREACH_ENUM_PARAM_7(func, __VA_ARGS__))
#define FOREACH_ENUM_PARAM_9(func, param, ...) func(param), TOML_EXPAND(FOREACH_ENUM_PARAM_8(func, __VA_ARGS__))
#define FOREACH_ENUM_PARAM_10(func, param, ...) func(param), TOML_EXPAND(FOREACH_ENUM_PARAM_9(func, __VA_ARGS__))
#define FOREACH_ENUM_PARAM_11(func, param, ...) func(param), TOML_EXPAND(FOREACH_ENUM_PARAM_10(func, __VA_ARGS__))
#define FOREACH_ENUM_PARAM_12(func, param, ...) func(param), TOML_EXPAND(FOREACH_ENUM_PARAM_11(func, __VA_ARGS__))
#define FOREACH_ENUM_PARAM_13(func, param, ...) func(param), TOML_EXPAND(FOREACH_ENUM_PARAM_12(func, __VA_ARGS__))
#define FOREACH_ENUM_PARAM_14(func, param, ...) func(param), TOML_EXPAND(FOREACH_ENUM_PARAM_13(func, __VA_ARGS__))
#define FOREACH_ENUM_PARAM_15(func, param, ...) func(param), TOML_EXPAND(FOREACH_ENUM_PARAM_14(func, __VA_ARGS__))
#define FOREACH_ENUM_PARAM_16(func, param, ...) func(param), TOML_EXPAND(FOREACH_ENUM_PARAM_15(func, __VA_ARGS__))
#define FOREACH_ENUM_PARAM_17(func, param, ...) func(param), TOML_EXPAND(FOREACH_ENUM_PARAM_16(func, __VA_ARGS__))
#define FOREACH_ENUM_PARAM_18(func, param, ...) func(param), TOML_EXPAND(FOREACH_ENUM_PARAM_17(func, __VA_ARGS__))
#define FOREACH_ENUM_PARAM_19(func, param, ...) func(param), TOML_EXPAND(FOREACH_ENUM_PARAM_18(func, __VA_ARGS__))
#define FOREACH_ENUM_PARAM_20(func, param, ...) func(param), TOML_EXPAND(FOREACH_ENUM_PARAM_19(func, __VA_ARGS__))
#define FOREACH_ENUM_PARAM_21(func, param, ...) func(param), TOML_EXPAND(FOREACH_ENUM_PARAM_20(func, __VA_ARGS__))


#define GET_FOREACH_MACRO(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,NAME,...) NAME
#define FOREACH_PARAM(func, ...) TOML_EXPAND(GET_FOREACH_MACRO(__VA_ARGS__, FOREACH_PARAM_21, FOREACH_PARAM_20, FOREACH_PARAM_19, FOREACH_PARAM_18, FOREACH_PARAM_17, FOREACH_PARAM_16, FOREACH_PARAM_15, FOREACH_PARAM_14, FOREACH_PARAM_13, FOREACH_PARAM_12, FOREACH_PARAM_11, FOREACH_PARAM_10, FOREACH_PARAM_9, FOREACH_PARAM_8, FOREACH_PARAM_7, FOREACH_PARAM_6, FOREACH_PARAM_5, FOREACH_PARAM_4, FOREACH_PARAM_3, FOREACH_PARAM_2, FOREACH_PARAM_1)(func, __VA_ARGS__))
#define FOREACH_ENUM_PARAM(func, ...) TOML_EXPAND(GET_FOREACH_MACRO(__VA_ARGS__, FOREACH_ENUM_PARAM_21, FOREACH_ENUM_PARAM_20, FOREACH_ENUM_PARAM_19, FOREACH_ENUM_PARAM_18, FOREACH_ENUM_PARAM_17, FOREACH_ENUM_PARAM_16, FOREACH_ENUM_PARAM_15, FOREACH_ENUM_PARAM_14, FOREACH_ENUM_PARAM_13, FOREACH_ENUM_PARAM_12, FOREACH_ENUM_PARAM_11, FOREACH_ENUM_PARAM_10, FOREACH_ENUM_PARAM_9, FOREACH_ENUM_PARAM_8, FOREACH_ENUM_PARAM_7, FOREACH_ENUM_PARAM_6, FOREACH_ENUM_PARAM_5, FOREACH_ENUM_PARAM_4, FOREACH_ENUM_PARAM_3, FOREACH_ENUM_PARAM_2, FOREACH_ENUM_PARAM_1)(func, __VA_ARGS__))

// clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
