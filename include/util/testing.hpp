/* testinh.hpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

namespace debug {

template <typename TClass> class Inspector;
}

/** \brief this macro adds a friend class to this class definition that
 * can be used in unit tests to view private variables */
#define TEST_INSPECTABLE(TClass) friend class debug::Inspector<TClass>;

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
