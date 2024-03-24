/* Application.hpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <vector>

#include "types.hpp"

namespace IOCore {
class Application // NOLINT(readability-identifier-naming)
{
public:
  virtual ~Application() = default;

  virtual auto init(int argc, c::const_string argv[], c::const_string envp[])
      -> void;

  virtual auto run() -> int = 0;

  [[nodiscard]] virtual auto getArguments() const
      -> const std::vector<std::string> & {
    return this->arguments;
  }
  [[nodiscard]] virtual auto getEnvironment() const
      -> const Dictionary<const std::string> & {
    return this->environment_variables;
  }

protected:
  Application();

  /// @{
  /// @name Disable copy and move operators / constructors
  Application(const Application &) = delete;
  auto operator=(const Application &) -> Application & = delete;
  /// @}

  void read_arguments(int argc, c::const_string argv[]);
  void create_env_dictionary(c::const_string envp[]);

  std::vector<std::string> arguments;
  Dictionary<const std::string> environment_variables;
};

} // namespace IOCore

// clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=4 sts=0 sw=4 noexpandtab ft=cpp.doxygen :
