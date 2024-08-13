/* TomlConfigFile.hpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "FileResource.hpp"
#include "TomlTable.hpp"

namespace IOCore {

class TomlConfigFile : public FileResource {
    public:
	TomlConfigFile(
	    const std::filesystem::path& file_path,
	    CreateDirs mode = CreateDirs::Disable
	);
	~TomlConfigFile() override;

	auto read() -> IOCore::TomlTable&;
	void write();

	template<typename T>
	[[nodiscard]] auto as() const -> T
	{
		return config_toml.as<T>();
	}
	template<typename T>
	[[nodiscard]] auto get() const -> T
	{
		return TomlConfigFile::as<T>();
	}

	template<typename T>
	void set(const T& value)
	{
		config_toml = value;
	}

	auto getTomlTable() -> IOCore::TomlTable& { return this->config_toml; }

    protected:
	IOCore::TomlTable config_toml;
};

} // namespace IOCore

// clang-format off
// vim: set foldmethod=syntax foldlevel=1 foldminlines=12 textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
