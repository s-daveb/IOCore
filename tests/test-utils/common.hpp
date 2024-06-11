/* test-includes.hpp
 * Copyright © 2020-2022 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "IOCore/sys/debuginfo.hpp"

#include <catch2/catch_test_macros.hpp>

#pragma GCC diagnostic ignored "-Wunused-variable"
#define BEGIN_TEST_SUITE(name)                                                  \
	static const char* TEST_SUITE_NAME = "[" name "]";                      \
	namespace

#define TEST(testname) TEST_CASE(testname, TEST_SUITE_NAME)

#define TEST_WITH_FIXTURE(FixtureName, testname)                                \
	TEST_CASE_METHOD(FixtureName, testname, TEST_SUITE_NAME)

#define FIXTURE_TEST(testname) TEST_WITH_FIXTURE(TestFixture, testname)

// clang-format off
// vim: set foldmethod=marker foldmarker=#region,#endregion textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :

