/* debuginfo.h
 * Copyright © 2020-2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#pragma once

#include <string>

namespace IOCore {

auto generate_stacktrace(unsigned short framesToRemove = 1) -> std::string;

void print_cmdline(int argc, const char* argv[]);

} // namespace IOCore

// clang-format off
// vim: set foldmethod=syntax  textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
