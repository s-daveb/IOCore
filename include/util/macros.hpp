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

#include <stdio.h>

#define IOCORE_EXPAND(x) x

#define IOCORE_FOREACH_1(func, param) func(param)
#define IOCORE_FOREACH_2(func, param, ...) func(param); IOCORE_EXPAND(IOCORE_FOREACH_1(func, __VA_ARGS__))
#define IOCORE_FOREACH_3(func, param, ...) func(param); IOCORE_EXPAND(IOCORE_FOREACH_2(func, __VA_ARGS__))
#define IOCORE_FOREACH_4(func, param, ...) func(param); IOCORE_EXPAND(IOCORE_FOREACH_3(func, __VA_ARGS__))
#define IOCORE_FOREACH_5(func, param, ...) func(param); IOCORE_EXPAND(IOCORE_FOREACH_4(func, __VA_ARGS__))
#define IOCORE_FOREACH_6(func, param, ...) func(param); IOCORE_EXPAND(IOCORE_FOREACH_5(func, __VA_ARGS__))
#define IOCORE_FOREACH_7(func, param, ...) func(param); IOCORE_EXPAND(IOCORE_FOREACH_6(func, __VA_ARGS__))
#define IOCORE_FOREACH_8(func, param, ...) func(param); IOCORE_EXPAND(IOCORE_FOREACH_7(func, __VA_ARGS__))
#define IOCORE_FOREACH_9(func, param, ...) func(param); IOCORE_EXPAND(IOCORE_FOREACH_8(func, __VA_ARGS__))
#define IOCORE_FOREACH_10(func, param, ...) func(param); IOCORE_EXPAND(IOCORE_FOREACH_9(func, __VA_ARGS__))
#define IOCORE_FOREACH_11(func, param, ...) func(param); IOCORE_EXPAND(IOCORE_FOREACH_10(func, __VA_ARGS__))
#define IOCORE_FOREACH_12(func, param, ...) func(param); IOCORE_EXPAND(IOCORE_FOREACH_11(func, __VA_ARGS__))
#define IOCORE_FOREACH_13(func, param, ...) func(param); IOCORE_EXPAND(IOCORE_FOREACH_12(func, __VA_ARGS__))
#define IOCORE_FOREACH_14(func, param, ...) func(param); IOCORE_EXPAND(IOCORE_FOREACH_13(func, __VA_ARGS__))
#define IOCORE_FOREACH_15(func, param, ...) func(param); IOCORE_EXPAND(IOCORE_FOREACH_14(func, __VA_ARGS__))
#define IOCORE_FOREACH_16(func, param, ...) func(param); IOCORE_EXPAND(IOCORE_FOREACH_15(func, __VA_ARGS__))
#define IOCORE_FOREACH_17(func, param, ...) func(param); IOCORE_EXPAND(IOCORE_FOREACH_16(func, __VA_ARGS__))
#define IOCORE_FOREACH_18(func, param, ...) func(param); IOCORE_EXPAND(IOCORE_FOREACH_17(func, __VA_ARGS__))
#define IOCORE_FOREACH_19(func, param, ...) func(param); IOCORE_EXPAND(IOCORE_FOREACH_18(func, __VA_ARGS__))
#define IOCORE_FOREACH_20(func, param, ...) func(param); IOCORE_EXPAND(IOCORE_FOREACH_19(func, __VA_ARGS__))
#define IOCORE_FOREACH_21(func, param, ...) func(param); IOCORE_EXPAND(IOCORE_FOREACH_20(func, __VA_ARGS__))

#define GET_FOREACH_MACRO(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,NAME,...) NAME
#define IOCORE_FOREACH(func, ...) IOCORE_EXPAND(GET_FOREACH_MACRO(__VA_ARGS__, IOCORE_FOREACH_21, IOCORE_FOREACH_20, IOCORE_FOREACH_19, IOCORE_FOREACH_18, IOCORE_FOREACH_17, IOCORE_FOREACH_16, IOCORE_FOREACH_15, IOCORE_FOREACH_14, IOCORE_FOREACH_13, IOCORE_FOREACH_12, IOCORE_FOREACH_11, IOCORE_FOREACH_10, IOCORE_FOREACH_9, IOCORE_FOREACH_8, IOCORE_FOREACH_7, IOCORE_FOREACH_6, IOCORE_FOREACH_5, IOCORE_FOREACH_4, IOCORE_FOREACH_3, IOCORE_FOREACH_2, IOCORE_FOREACH_1)(func, __VA_ARGS__))

// clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
