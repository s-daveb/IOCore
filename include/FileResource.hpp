/* FileResource.hpp
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "Exception.hpp"
#include "types.hpp"

#include <filesystem>
#include <fstream>

namespace IOCore {
enum class CreateDirs : bool {
	Default = false,
	Disable = false,
	Disabled = false,
	Enable = true,
	Enabled = true
};

class FileResource {
    public:
	FileResource(
	    const std::filesystem::path& file_path,
	    CreateDirs mode = CreateDirs::Default
	);

	virtual ~FileResource() = default;

    protected:
	std::filesystem::path file_path;
};

struct UnreachablePathException : public Exception {
	UnreachablePathException(const std::filesystem::path& path)
	    : Exception("Unreachable path or directory"), unreachable_path(path)
	{
		this->generate_final_what_message(
		    "IOCore::UnreachablePathException", path.c_str()
		);
	}

	auto what() const noexcept -> const char* override
	{
		return Exception::what();
	}

	std::filesystem::path unreachable_path;
};
} // namespace IOCore

// clang-format off
// vim: set foldmethod=syntax foldlevel=2 foldminlines=10 textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
