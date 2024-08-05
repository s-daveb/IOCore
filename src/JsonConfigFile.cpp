/* JsonConfigFile.cpp
 * Copyright © 202t4 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "JsonConfigFile.hpp"

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
	Compact = -1,
	NewlinesOnly = 0,
	Ident = 1,
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

JsonConfigFile::JsonConfigFile(const fs::path& file_path, CreateDirs mode)
    : FileResource(file_path, mode)
{
}

JsonConfigFile::~JsonConfigFile() = default;

auto JsonConfigFile::read() -> nlohmann::json&
{
	try {
		std::ifstream file_stream(file_path);

		if (!file_stream.is_open()) {
			error_buffer.str("");
			error_buffer
			    << "Error opening JsonConfigFile for reading: "
			    << file_path << std::endl;
			throw IOCore::Exception(error_buffer.str());
		}

		// If the file is empty, do not try to open it and parse JSON
		if (file_stream.peek() != std::ifstream::traits_type::eof()) {
			file_stream >> config_json;
		}
	} catch (IOCore::Exception& except) {
		throw;
	} catch (const std::exception& e) {
		error_buffer.str("");
		error_buffer << "JsonConfigFile::Read() error" << std::endl
			     << e.what() << std::flush;
		throw IOCore::Exception(error_buffer.str());
	}

	return this->config_json;
}

void JsonConfigFile::write()
{
	try {
		std::ofstream file_stream(file_path);
		if (!file_stream.is_open()) {
			error_buffer.str("");
			error_buffer << "Error opening JsonConfigFile "
					"for writing: "
				     << file_path << std::endl;
			throw IOCore::Exception(error_buffer.str());
		}

		file_stream << config_json.dump(
				   IndentMode::Ident,
				   '\t',
				   AsciiMode::IgnoreUnicode,
				   nlohmann::json::error_handler_t::replace
			       )
			    << std::endl;

		return;
	} catch (IOCore::Exception& except) {
		throw;
	} catch (const std::exception& e) {

		error_buffer.str("");
		error_buffer << "JSonConfigFile::Save() error: " << std::endl
			     << e.what() << std::flush;
		throw IOCore::Exception(error_buffer.str());
	}
}
} // namespace elemental::configuration

// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
