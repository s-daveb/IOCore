/* Application.hpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <atomic>
#include <format>
#include <string>
#include <vector>

#include <fmt/core.h>

#include "Exception.hpp"

#include "types.hpp"

namespace IOCore {

struct DoubleConstructionException;

class Application // NOLINT(readability-identifier-naming)
{
    public:
	virtual ~Application();

	virtual auto run() -> int = 0;

	[[nodiscard]] virtual auto getArguments() const
	    -> const std::vector<std::string>&
	{
		return this->arguments;
	}
	[[nodiscard]] virtual auto getEnvironment() const
	    -> const Dictionary<const std::string>&
	{
		return this->environment_variables;
	}

    protected:
	Application(int argc, c::const_string argv[], c::const_string env[]);

	/// @{
	/// @name Disable copy and move operators / constructors
	Application(const Application&) = delete;
	auto operator=(const Application&) -> Application& = delete;
	/// @}

	void read_arguments(int argc, c::const_string argv[]);
	void create_env_dictionary(c::const_string envp[]);

	std::vector<std::string> arguments;
	Dictionary<const std::string> environment_variables;

    private:
	static std::atomic_bool is_initialized;
};

struct DoubleConstructionException : public IOCore::Exception {
	DoubleConstructionException(const std::string& classname)
	    : IOCore::Exception(fmt::format(
		  "A constructor was called twice for {}.", classname.c_str()
	      ))
	{
	}
};
} // namespace IOCore

// clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
