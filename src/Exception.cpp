/* exceptions.hpp
 * Copyright © 2020 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "Exception.hpp"

#include "sys/debuginfo.hpp"
#include "types.hpp"

#include <algorithm>
#include <exception>
#include <optional>
#include <string>

#include <fmt/core.h>

using IOCore::Exception;

constexpr unsigned kDEFAULT_STACKFRAMES_TO_STRIP = 3;

// Helper classes and functions. #region

Exception::Exception(c::const_string error_message)
    : std::exception()
    , error_message(error_message)
    , what_message()
    , stack_trace(IOCore::generate_stacktrace(kDEFAULT_STACKFRAMES_TO_STRIP))
    , inner_exception_ptr()
{
	generate_final_what_message();
}

Exception::Exception(const std::string& error_message)
    : std::exception()
    , error_message(std::move(error_message))
    , what_message()
    , stack_trace(IOCore::generate_stacktrace(kDEFAULT_STACKFRAMES_TO_STRIP))
    , inner_exception_ptr()
{
	generate_final_what_message();
}

Exception::Exception(const std::exception& inner)
    : std::exception(inner)
    , error_message(inner.what())
    , what_message()
    , inner_exception_ptr(std::make_exception_ptr(&inner))
    , stack_trace(IOCore::generate_stacktrace(kDEFAULT_STACKFRAMES_TO_STRIP))
{
	generate_final_what_message();
}

auto Exception::what() const noexcept -> const char*
{
	return this->what_message.c_str();
}

auto Exception::stacktrace() const noexcept -> const std::string&
{
	return this->stack_trace;
}

auto prepend_tabs_to_lines(const std::string& input) -> std::string
{
	std::ostringstream results_buffer;
	std::istringstream input_buffer(input);

	// Function to add a tab character before each line
	auto add_tab_before_line = [&results_buffer](const std::string& line) {
		results_buffer << '\t' << line << '\n';
	};

	// Process each line and add a tab character before it
	std::string line;
	while (std::getline(input_buffer, line)) {
		add_tab_before_line(line);
	}

	return results_buffer.str();
}
void Exception::generate_final_what_message(
    c::const_string class_name, c::const_string optional_data
)
{
	std::string my_name(class_name);

	if (my_name.empty()) {
		my_name = "IOCore::Exception";
	}

	if (std::strlen(optional_data) <= 0) {
		optional_data = "(null)";
	}

	std::string indented_stacktrace =
	    prepend_tabs_to_lines(this->stack_trace);

	this->what_message = fmt::format(
	    "{}::what(): {{\n"
	    "\terror: {}\n"
	    "\tdata: {}\n"
	    "\tstack_trace:{}\n"
	    "}};",
	    my_name.c_str(),
	    error_message.c_str(),
	    optional_data,
	    indented_stacktrace
	);
}

// clang-format off
// vim: set foldmethod=marker foldmarker=\{,\} textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :
