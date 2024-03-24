/* containers.hpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <string>
#include <unordered_map>

namespace IOCore {

/// \brief Convenience for std::map objects where the key is always a string.
template <typename TValueT>
using Dictionary = std::unordered_map<std::string, TValueT>;
} // namespace IOCore

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
