/* IApplication.hpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "types.hpp"

#include <functional>
#include <vector>

namespace elemental {

struct IApplication;
using IApplicationRef = std::reference_wrapper<IApplication>;

class IApplication
{
  public:
	virtual ~IApplication() = default;

	virtual auto init(int argc, c::const_string argv[],
	                  c::const_string envp[]) -> void = 0;
	virtual auto run() -> int = 0;

	[[nodiscard]] virtual auto getArguments() const
	    -> const std::vector<std::string>& = 0;
	[[nodiscard]] virtual auto getEnvironment() const
	    -> const Dictionary<const std::string>& = 0;

  protected:
	IApplication() {}
};

} // namespace elemental

// clang-format off
// vim: set foldmethod=syntax  textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
