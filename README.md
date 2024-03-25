
# IOCore: A C++20 Library

IOCore is a comprehensive C++20 library designed to simplify common programming tasks. It features advanced argument parsing functions, a custom exception class that captures stack traces at the point of throw, and a set of convenience macros for writing unit tests, among other utilities. The library leverages modern C++20 features and best practices to provide a robust foundation for software development.

## Getting Started
### Prerequisites

- CMake (version 3.21 or higher)
- A C++20-compatible compiler
- Optional: Catch2 installed on your system to speed up compile time

### Building the Project
1. Clone the repository:
    ```
    git clone https://gitea.beniquez.me/sdaveb/IOCore.git
    ```

2. Navigate to the project directory:
    ```
    cd IOCore
    ```
3. Configure your build system:
    ```bash
    cmake -B debug -G Unix Makefiles
    # or
    cmake -B debug -G Ninja 
    ```
4. Invoke your build system:
    ```
    cmake --build debug
    ```

### Running Unit Tests

1. After building the project, you can run the unit tests:
    ``` 
    cmake --build debug --target ctest
    ```
This will execute the IOCore test suite using Catch2.

## Project Structure
- `src/`: Contains the core library code, including argument parsing functions and the custom exception class.
- `app/`: Provides examples on how to utilize the IOCore library in applications.
- `test/`: Contains unit tests for the library, leveraging Catch2 and FakeIt to ensure robustness and reliability.

## Key Features
- **Argument Parsing**: Simplifies handling command-line arguments with a flexible and intuitive API.
- **Exception Handling**: Offers a powerful exception class that includes stack traces for comprehensive error analysis.
- **Unit Testing Macros**: Facilitates writing unit tests by providing convenient macros that integrate seamlessly with Catch2.

## Dependencies

- **Catch2 v3**: Test framework for C++ (can be fetched using FetchContent).
- **FakeIt**: Mocking framework for C++ (also fetched using FetchContent).

## License
This project is licensed under the MPL2.0 License - see the [LICENSE](LICENSE) file for details.

