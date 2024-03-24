/* Application.test.cpp
 * Copyright © 2023 Saul D. Beniquez
 * License: Mozilla Public License v. 2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public License,
 * v.2.0. If a copy of the MPL was not distributed with this file, You can
 * obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "Application.hpp"
#include "Exception.hpp"

#include "test-utils/common.hpp"
#include <catch2/matchers/catch_matchers_string.hpp>

namespace {
struct SimulatedLaunch {
	static const char* argv[];
	static const char* env[];
};
inline const char* SimulatedLaunch::argv[] = { "param1", "param2", "param3" };
inline const char* SimulatedLaunch::env[] = {
	"PATH=/usr/bin",
	"VAR2=TWO",
	"REQUEST_URI=markdown?msg=hello-world",
	nullptr
};
} // namespace

BEGIN_TEST_SUITE("core::Application")
{
	using namespace core;
	struct MockApplicationClass : public Application {
		auto run() -> int override { return 0; }
	};
	struct TestFixture {
		TestFixture() : derived_app(), app(derived_app) {}

		MockApplicationClass derived_app;
		Application& app;
	};

	FIXTURE_TEST("core::Application - Init method populates "
	             "Arguments list and Environment dictionary")
	{
		app.init(3, SimulatedLaunch::argv, SimulatedLaunch::env);

		CHECK(app.getArguments().size() == 3);
		CHECK(app.getArguments()[0] == "param1");
		CHECK(app.getArguments()[1] == "param2");
		CHECK(app.getArguments()[2] == "param3");

		CHECK(app.getEnvironment().at("PATH") == "/usr/bin");
		CHECK(app.getEnvironment().at("VAR2") == "TWO");
		CHECK(
		    app.getEnvironment().at("REQUEST_URI") ==
		    "markdown?msg=hello-world"
		);
	};
}
// clang-format off
// vim: set foldmethod=marker foldmarker=#region,#endregion textwidth=80 ts=8 sts=0 sw=8  noexpandtab ft=cpp.doxygen :
