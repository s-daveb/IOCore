/* serialization.hpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */
#pragma once

#include <nlohmann/json.hpp>

/* NOLINTBEGIN(readability-identifier-length) */

#define SERIALIZABLE(...) NLOHMANN_DEFINE_TYPE_INTRUSIVE(__VA_ARGS__)
#define SERIALIZABLE_ENUM(...) NLOHMANN_JSON_SERIALIZE_ENUM(__VA_ARGS__)

/* NOLINTEND */

// clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
