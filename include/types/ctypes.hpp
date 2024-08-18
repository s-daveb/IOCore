/* legible_ctypes.hpp
 * Copyright © 2023-2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <cstddef>

/*! @name Aliases for C-types that are unclear.
 * Examples: The C type `char*` does not communicate that it's being used
 * to represent a string; socket libraries return `int`,
 * rather than a typedef socket_fd_t, etc... */
namespace c {
using mutable_string = char*;
using immutable_string = const char*;
using string_constant = const char* const;

using string = immutable_string;
using const_string = immutable_string; //! @todo: delete
// using const_string = string_constant; //! @todo uncomment

using count_t = size_t;
} // namespace c

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
