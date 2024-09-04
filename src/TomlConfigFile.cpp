/* TomlConfigFile.cpp
 * Copyright © 202t4 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "TomlConfigFile.hpp"
#include "util/toml.hpp"

#include "Exception.hpp"
#include "sys/debuginfo.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

#include <nlohmann/json.hpp>

namespace fs = std::filesystem;

namespace impl {
static std::stringstream error_buffer;

enum IndentMode : int {
	Disabled = 0,
	Enabled = 1,
};
enum AsciiMode : bool {
	Default = false,
	Raw = false,
	IgnoreUnicode = false,
	EscapeUnicode = true
};
} // namespace

namespace IOCore {
using namespace impl;

TomlConfigFile::TomlConfigFile(const fs::path& file_path, CreateDirs mode)
    : FileResource(file_path, mode)
{
}

TomlConfigFile::~TomlConfigFile() = default;

auto TomlConfigFile::read() -> IOCore::TomlTable&
{
	try {
		std::ifstream file_stream(file_path);

		if (!file_stream.is_open()) {
			error_buffer.str("");
			error_buffer
			    << "Error opening TomlConfigFile for reading: "
			    << file_path << std::endl;
			throw IOCore::Exception(error_buffer.str());
		}

		//// If the file is empty, do not try to open it and parse TOML
		if (file_stream.peek() != std::ifstream::traits_type::eof()) {
			config_toml = toml::parse(file_stream);
		}
	} catch (IOCore::Exception& except) {
		throw;
	} catch (const std::exception& e) {
		error_buffer.str("");
		error_buffer << "TomlConfigFile::Read() error" << std::endl
			     << e.what() << std::flush;
		throw IOCore::Exception(error_buffer.str());
	}

	return this->config_toml;
}

void TomlConfigFile::write()
{
	using toml::format_flags;
	using toml::toml_formatter;

	try {
		std::ofstream file_stream(file_path);
		if (!file_stream.is_open()) {
			error_buffer.str("");
			error_buffer << "Error opening TomlConfigFile "
					"for writing: "
				     << file_path << std::endl;
			throw IOCore::Exception(error_buffer.str());
		}

		auto formatter =
		    toml_formatter{ config_toml,
			            toml_formatter::default_flags &
			                ~format_flags::indent_sub_tables };
		file_stream << formatter << std::endl;

		return;
	} catch (IOCore::Exception& except) {
		throw;
	} catch (const std::exception& e) {

		error_buffer.str("");
		error_buffer << "TomlConfigFile::Save() error: " << std::endl
			     << e.what() << std::flush;
		throw IOCore::Exception(error_buffer.str());
	}
}
} // namespace elemental::configuration

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :

