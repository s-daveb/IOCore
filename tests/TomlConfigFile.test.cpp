/* TomlConfigFile.test.cpp
 * }
 * Copyright © 2024 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "IOCore/TomlConfigFile.hpp"
#include "IOCore/Exception.hpp"
#include "IOCore/util/toml.hpp"

#include "IOCore/sys/debuginfo.hpp"
#include "IOCore/util/debug_print.hpp"

#include "test-utils/common.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include <nlohmann/json.hpp>

namespace fs = std::filesystem;

constexpr c::const_string kINPUT_FILE_PATH = "/tmp/test_config.toml";
constexpr c::const_string kBADFILE_PATH = "/tmp/test_bad_config.toml";
constexpr c::const_string kNON_EXISTENT_PATH = "/hades/tmp/dne/file.toml";

using TomlTable = IOCore::TomlTable;
using TomlConfigFile = IOCore::TomlConfigFile;
using DataDict = IOCore::Dictionary<std::string>;

template<>
auto to_toml_table<DataDict>(const DataDict& data) -> toml::table
{
	toml::table result;
	for (const auto& [key, value] : data) {
		result.insert_or_assign(key, value);
	}
	return result;
}

BEGIN_TEST_SUITE("elemental::TomlConfigFile")
{

	namespace { // Test fixtures
	struct SampleFileGenerator {
		SampleFileGenerator()
		{
			if (!fs::exists(kINPUT_FILE_PATH)) {
				std::ofstream new_file(
				    kINPUT_FILE_PATH,
				    std::ios::out | std::ios::trunc
				);
				new_file << R"({key1 = "value1"
				           key2 = "value2"})"
					 << std::endl;
				new_file.close();
			}
		}
		~SampleFileGenerator()
		{
			try {
				if (fs::exists(kINPUT_FILE_PATH)) {
					fs::remove(kINPUT_FILE_PATH);
				}
			} catch (std::exception& e) {
				DEBUG_PRINT(e.what());
			}
		}
	};
	using TestFixture = SampleFileGenerator;
	} // anonymous namespace

	TEST("elemental::toml::table is serializablable like "
	     "std::map<std::string,std::string>")
	{
		IOCore::Dictionary<std::string> test_data;
		IOCore::TomlTable tomlized;

		test_data["one"] = "1";
		test_data["resolution"] = "1280x720";
		test_data["Hello"] = "world";

		tomlized = test_data;

		REQUIRE(test_data["one"] == tomlized["one"].ref<std::string>());
		REQUIRE(
		    test_data["resolution"] ==
		    tomlized["resolution"].ref<std::string>()
		);
		REQUIRE(
		    test_data["Hello"] == tomlized["Hello"].ref<std::string>()
		);
	}

	FIXTURE_TEST("TomlConfigFile construction")
	{
		SECTION("TomlConfigFile w/ valid path")
		{
			auto config = TomlConfigFile(kINPUT_FILE_PATH);
			auto& config_data = config.getTomlTable();

			REQUIRE(config_data.empty());
		}
		SECTION("TomlConfigFile w/ invalid path throws exception")
		{

			REQUIRE_THROWS_AS(
			    TomlConfigFile(kNON_EXISTENT_PATH),
			    IOCore::UnreachablePathException
			);
		}
	}
	FIXTURE_TEST("TomlConfigFile::Read")
	{
		auto config_file = TomlConfigFile(kINPUT_FILE_PATH);
		auto& json_data = config_file.getTomlTable();

		SECTION("TomlConfigFile::Read w/ valid file")
		{
			config_file.read();
			REQUIRE_FALSE(json_data.empty());
			REQUIRE(json_data.size() == 2);

			REQUIRE(json_data["key1"] == "value1");
			REQUIRE(json_data["key2"] == "value2");
		}
		SECTION("TomlConfigFile::Read w/ bad file throws exception")
		{
			if (!fs::exists(kBADFILE_PATH)) {
				std::ofstream fileout(kBADFILE_PATH);
				fileout << R"({"Hello World"})" << std::endl;
				fileout.close();
			}
			REQUIRE_THROWS_AS(
			    [&]() {
				    auto bad_config =
					TomlConfigFile(kBADFILE_PATH);

				    bad_config.read();
			    }(),
			    IOCore::Exception
			);
		}
		if (fs::exists(kBADFILE_PATH)) {
			fs::remove(kBADFILE_PATH);
		}
	} /*
	 TEST("TomlConfigFile::Write()")
	 {
	         SECTION("Create File and Read It back In")
	         {
	                 auto config_file = TomlConfigFile(kINPUT_FILE_PATH);
	                 auto& test_data = config_file.getTomlTable();

	                 test_data.insert("one", "1");
	                 test_data.insert("resolution", "1280x720");
	                 test_data.insert("Hello", "world");

	                 config_file.write();

	                 std::ifstream resulting_file(kINPUT_FILE_PATH);
	                 IOCore::TomlTable toml_object;

	                 resulting_file >> toml_object;
	                 auto written_data =
	                     toml_object.ref<IOCore::Dictionary<std::string>>();

	                 REQUIRE(
	                     written_data["one"] ==
	                     test_data["one"].ref<std::string>()
	                 );
	                 REQUIRE(
	                     written_data["resolution"] ==
	                     test_data["resolution"].ref<std::string>()
	                 );
	                 REQUIRE(
	                     written_data["Hello"] ==
	                     test_data["Hello"].ref<std::string>()
	                 );
	         }
	         fs::remove(kINPUT_FILE_PATH);
	 }

	 FIXTURE_TEST(
	     "TomlConfigFile::Get<T>() basically wraps toml::table::get<T>()"
	 )
	 {
	         auto config_file = TomlConfigFile(kINPUT_FILE_PATH);
	         auto& json_data = config_file.getTomlTable();

	         config_file.read();
	         auto obtained_data =
	             config_file.get<IOCore::Dictionary<std::string>>();

	         REQUIRE(obtained_data["key1"] == "value1");
	         REQUIRE(obtained_data["key2"] == "value2");
	 }
	 FIXTURE_TEST(
	     "TomlConfigFile::Set() basically wraps toml::table::operator=()"
	 )
	 {
	         auto config_file = TomlConfigFile(kINPUT_FILE_PATH);
	         IOCore::Dictionary<std::string> test_data;

	         test_data["one"] = "1";
	         test_data["resolution"] = "1280x720";
	         test_data["Hello"] = "world";


	         REQUIRE(json_data["one"] == test_data["one"]);
	         REQUIRE(json_data["resolution"] == test_data["resolution"]);
	         REQUIRE(json_data["Hello"] == test_data["Hello"]);
	 } */
}
// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
