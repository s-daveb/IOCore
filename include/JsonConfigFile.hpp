/* JsonConfigFile.hpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "FileResource.hpp"

#include "types.hpp"

#include <filesystem>

#include <nlohmann/json.hpp>

namespace elemental::configuration {

class JsonConfigFile : public FileResource
{
  public:
	JsonConfigFile(const std::filesystem::path& file_path,
	               CreateDirs mode = CreateDirs::Disable);
	~JsonConfigFile() override;

	auto read() -> nlohmann::json&;
	void write();

	template<typename T_>
	[[nodiscard]] auto get() const -> T_
	{
		return config_json.get<T_>();
	}

	template<typename T_>
	void set(const T_& value)
	{
		config_json = value;
	}

	auto getJsonData() -> nlohmann::json& { return this->config_json; };

  protected:
	nlohmann::json config_json;
};
} // namespace elemental::configuration

// clang-format off
// vim: set foldmethod=syntax foldlevel=1 foldminlines=12 textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
