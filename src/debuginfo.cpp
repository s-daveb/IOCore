/* debuginfo.h
 * Copyright © 2020-2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "sys/debuginfo.hpp"
#include "sys/platform.hpp"

#if !defined(BOOST_STACKTRACER)
#if defined(HAVE_EXECINFO_H)
#include <execinfo.h>
#endif
#include <cxxabi.h>
#else
#include <boost/stacktrace.hpp>
#endif

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace IOCore {
namespace {

// Extracts the mangled symbol from a string like <func_name+0x34>
auto extract_mangled_symbol(const std::string& input) -> std::string
{
	std::string result;
	bool inside_angle_brackets = false;

	for (char symbol : input) {
		if (symbol == '<') {
			inside_angle_brackets = true;
			continue;
		}
		if (symbol == '>') {
			inside_angle_brackets = false;
			continue;
		}
		if (symbol == '+') {
			break;
		}

		if (inside_angle_brackets) {
			result += symbol;
		}
	}

	return result;
}
} // namespace

// There are a lot of C and platform-specific hacks contained within
// I am sorry. 🤡
auto generate_stacktrace(unsigned short framesToRemove) -> std::string
{

	std::stringstream buffer;

#ifndef BOOST_STACKTRACER
	void* callstack[128];
	int i, frames = backtrace(callstack, 128);
	char** strs = backtrace_symbols(callstack, frames);

	size_t columns_to_print = 0;

	// preconfigure column length for certain platforms
	if (kCurrentPlatform == Platform::FreeBSD) {
		columns_to_print = 2;
	} else if (kCurrentPlatform == Platform::macOS) {
		columns_to_print = 4;
	}

	if (framesToRemove == frames) {
		framesToRemove = 0;
	}

	for (i = framesToRemove; i < frames; ++i) {
		std::string word;
		std::stringstream line_stream(strs[i]);
		std::vector<std::string> wordlist;

		// Create a list of words for this stack trace line
		while (line_stream >> word) {
			if (columns_to_print != 0 &&
			    (word.find('<') != word.npos &&
			     word.find('>') != word.npos)) {
				auto extracted_symbol =
				    extract_mangled_symbol(word);
				word = extracted_symbol;
			}
			wordlist.push_back(word);
		}
		// if columns_to_print is still 0, assign it to the list length
		// It is only pre-configured for certain platforms, see above
		if (!columns_to_print) {
			columns_to_print = wordlist.size();
		}
		// Process the extracted words one at a time and format the
		// stack trace string
		for (unsigned pos = 0; pos < columns_to_print; ++pos) {
			auto word = wordlist[pos];
			int status;

			char* demangled_symbol = abi::__cxa_demangle(
			    word.c_str(), nullptr, nullptr, &status
			);

			if (status == 0) {
				buffer << demangled_symbol << '\t';
				std::free(demangled_symbol);
			} else {
				buffer << word << '\t';
			}
		}
		buffer << std::endl;
	}
	std::free(strs);
#else
	buffer << boost::stacktrace::stacktrace() << std::flush;
#endif
	return buffer.str();
}

void print_cmdline(int argc, const char* argv[])
{
	int i;

	std::cout << "Command-line received" << std::endl;
	for (i = 0; i < argc; ++i)
		std::cout << argv[i] << " ";
	std::cout << std::endl;
}

} // namespace IOCore

// clang-format off
// vim: set foldmethod=syntax foldminlines=10 textwidth=80 ts=8 sts=0 sw=8 noexpandtab ft=cpp.doxygen :

