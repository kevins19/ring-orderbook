cmake_minimum_required(VERSION 3.20)

project(trading_bot VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Use Homebrew GCC
set(CMAKE_C_COMPILER /opt/homebrew/bin/g++-14)
set(CMAKE_CXX_COMPILER /opt/homebrew/bin/g++-14)

# Include directories
include_directories(include)
include_directories(/opt/homebrew/Cellar/websocketpp/0.8.2/include)
include_directories(/opt/homebrew/include)

# Find packages
find_package(Boost REQUIRED COMPONENTS system thread)
find_package(OpenSSL REQUIRED)

# Collect all strategy source files
file(GLOB STRATEGY_SOURCES "src/strategies/*.cpp")
file(GLOB DATA_STRUCTURES "src/data_structures/*.cpp")

# Add the library
add_library(trading_lib
    src/listener.cpp
    src/runner.cpp
    ${STRATEGY_SOURCES}
    ${DATA_STRUCTURES}
)

# Add the example executable
add_executable(example examples/main.cpp)

# Link the library to the example executable
target_link_libraries(trading_lib
    Boost::system
    Boost::thread
    OpenSSL::SSL
    OpenSSL::Crypto
)

# Link the library to the example executable
target_link_libraries(example PRIVATE trading_lib)
