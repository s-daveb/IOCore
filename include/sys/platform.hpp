/* platform.hpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */
#pragma once

namespace core {

enum class Platform : int {
	Unknown = 0b0000,
	Windows = 0b0001,
	Unix = 0b0100,
	Linux = 0b0101,
	FreeBSD = 0b0110,
	macOS = 0b0111 // NOLINT
};

#ifdef __linux__
static const Platform kCurrentPlatform = Platform::Linux;
#endif

#ifdef _WIN32
static const Platform kCurrentPlatform = Platform::Windows;
#endif

#ifdef __APPLE__
static const Platform kCurrentPlatform = Platform::macOS;
#endif

#ifdef __FreeBSD__
static const Platform kCurrentPlatform = Platform::FreeBSD;
#endif

// I don't own any AIX, Solaris, HP-UX, or pure Darwin systems, sorry :)
// To be added at a later date.

} // namespace core::platform

// clang-format off
// vim: set foldmethod=syntax  textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
