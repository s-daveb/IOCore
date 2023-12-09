C++20 CMake Project Template

This is a simple C++20 project template using CMake for building and Catch2 v3 along with FakeIt for unit testing. The project is set up to fetch Catch2 and FakeIt using CMake's FetchContent command if they are not found on the system.

Getting Started

Prerequisites

- CMake (version 3.21 or higher)
- C++20-compatible compiler

Building the Project

1. Clone the repository:

    git clone https://github.com/your-username/your-project.git

2. Navigate to the project directory:

    cd your-project

3. Create a build directory:

    mkdir build && cd build

4. Configure and build the project using CMake:

    cmake ..
    make

Running Unit Tests

After building the project, you can run the unit tests:

make test

This will execute the Catch2 test suite.

Project Structure

- src/: Contains the source code for your application.
- test/: Contains unit tests using Catch2 and FakeIt.

Dependencies

- Catch2 v3: Test framework for C++ (fetched using FetchContent).
- FakeIt: Mocking framework for C++ (fetched using FetchContent).

License

This project is licensed under the MIT License.

