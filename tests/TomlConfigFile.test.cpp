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

c::string_constant kInputFilePath = "/tmp/test_config.toml";
c::string_constant kBadFilePath = "/tmp/test_bad_config.toml";
c::string_constant kNonExistantPath = "/hades/tmp/dne/file.toml";

using TomlTable = IOCore::TomlTable;
using TomlConfigFile = IOCore::TomlConfigFile;
using StringDictionary = IOCore::Dictionary<std::string>;
using Serializer = IOCore::TOML::Serializer;

IOCORE_TOML_SERIALIZE_IMPL(StringDictionary)
{
	toml::table result;
	for (const auto& [key, value] : obj) {
		result.insert_or_assign(key, value);
	}
	return result;
}
IOCORE_TOML_DESERIALIZE_IMPL(StringDictionary)
{
	result.clear();
	for (auto [key, value] : tbl) {
		result.emplace(key, value.ref<std::string>());
	}
}
struct SimpleStruct {
	int field1;
	int field2;

	auto operator==(const SimpleStruct& other) const -> bool
	{
		return field1 == other.field1 && field2 == other.field2;
	}

	IOCORE_TOML_SERIALIZABLE(SimpleStruct, field1, field2);
};

struct ComplexStruct {
	SimpleStruct field1;
	int field2;

	IOCORE_TOML_SERIALIZABLE(ComplexStruct, field1, field2);
};

BEGIN_TEST_SUITE("elemental::TomlConfigFile")
{

	namespace { // Test fixtures
	struct SampleFileGenerator {
		SampleFileGenerator()
		{
			if (!fs::exists(kInputFilePath)) {
				std::ofstream new_file(
				    kInputFilePath,
				    std::ios::out | std::ios::trunc
				);
				new_file << R"(key1 = "value1")" << std::endl
					 << R"(key2 = "value2")" << std::endl;
				new_file.close();
			}
		}
		~SampleFileGenerator()
		{
			try {
				if (fs::exists(kInputFilePath)) {
					fs::remove(kInputFilePath);
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
		StringDictionary test_data;
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
			auto config = TomlConfigFile(kInputFilePath);
			auto& config_data = config.getTomlTable();

			REQUIRE(config_data.empty());
		}
		SECTION("TomlConfigFile w/ invalid path throws exception")
		{
			REQUIRE_THROWS_AS(
			    TomlConfigFile(kNonExistantPath),
			    IOCore::UnreachablePathException
			);
		}
	}

	FIXTURE_TEST("TomlConfigFile::Read")
	{
		auto config_file = TomlConfigFile(kInputFilePath);
		auto& toml_data = config_file.getTomlTable();

		SECTION("TomlConfigFile::Read w/ valid file")
		{
			config_file.read();
			REQUIRE_FALSE(toml_data.empty());
			REQUIRE(toml_data.size() == 2);

			REQUIRE(toml_data["key1"] == "value1");
			REQUIRE(toml_data["key2"] == "value2");
		}
		SECTION("TomlConfigFile::Read w/ bad file throws exception")
		{
			if (!fs::exists(kBadFilePath)) {
				std::ofstream fileout(kBadFilePath);
				fileout << R"("Hello World")" << std::endl;
				fileout.close();
			}
			REQUIRE_THROWS_AS(
			    [&]() {
				    auto bad_config =
					TomlConfigFile(kBadFilePath);

				    bad_config.read();
			    }(),
			    IOCore::Exception
			);
		}
		if (fs::exists(kBadFilePath)) {
			fs::remove(kBadFilePath);
		}
	}
	TEST("TomlConfigFile::Write()")
	{
		SECTION("Create File and Read It back In")
		{
			auto config_file = TomlConfigFile(kInputFilePath);
			auto& test_data = config_file.getTomlTable();

			test_data.insert("one", "1");
			test_data.insert("resolution", "1280x720");
			test_data.insert("Hello", "world");

			config_file.write();

			std::ifstream resulting_file(kInputFilePath);
			IOCore::TomlTable toml_object;

			resulting_file >> toml_object;

			auto written_data =
			    toml_object.as<IOCore::Dictionary<std::string>>();

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
		fs::remove(kInputFilePath);
	}

	FIXTURE_TEST(
	    "TomlConfigFile::Get<T>() basically wraps toml::table::get<T>() "
	)
	{
		auto config_file = TomlConfigFile(kInputFilePath);
		auto& json_data = config_file.getTomlTable();

		config_file.read();
		auto obtained_data =
		    config_file.get<IOCore::Dictionary<std::string>>();

		REQUIRE(obtained_data["key1"] == "value1");
		REQUIRE(obtained_data["key2"] == "value2");
	}
	FIXTURE_TEST(
	    "TomlConfigFile::Set() basically wraps toml::table::operator=() "
	)
	{
		auto config_file = TomlConfigFile(kInputFilePath);
		IOCore::Dictionary<std::string> expected_data;

		expected_data["one"] = "1";
		expected_data["resolution"] = "1280x720";
		expected_data["Hello"] = "world";

		config_file.set(expected_data);
		auto& toml_data = config_file.getTomlTable();

		REQUIRE(
		    toml_data["one"].value<std::string>() == expected_data["one"]
		);
		REQUIRE(
		    toml_data["resolution"].value<std::string>() ==
		    expected_data["resolution"]
		);
		REQUIRE(
		    toml_data["Hello"].value<std::string>() ==
		    expected_data["Hello"]
		);
	}
	FIXTURE_TEST("TomlConfigFile works with ComplexStruct ")
	{
		auto config_file = TomlConfigFile(kInputFilePath);
		config_file.set(ComplexStruct{ { 11, 22 }, 30 });
		auto toml_data = config_file.getTomlTable();

		REQUIRE(toml_data["field1"]["field1"].value<int>() == 11);
		REQUIRE(toml_data["field1"]["field2"].value<int>() == 22);
		REQUIRE(toml_data["field2"].value<int>() == 30);
	}
}
// clang-format off
// vim: set foldmethod=syntax textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
