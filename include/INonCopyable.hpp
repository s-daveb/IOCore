/* INoncopyable.hpp
 * Copyright © 2018-2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

namespace Core {

class INoncopyable {
protected:
  INoncopyable() = default;
  virtual ~INoncopyable() = default;

private:
  INoncopyable(const INoncopyable &) = default;
  INoncopyable(INoncopyable &&) = default;
  auto operator=(const INoncopyable &) -> INoncopyable & = default;
  auto operator=(INoncopyable &&) -> INoncopyable & = default;
};
} // namespace Core

// clang-format off
// vim: set foldmethod=marker foldmarker=#region,#endregion textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
