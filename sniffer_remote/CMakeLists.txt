cmake_minimum_required(VERSION 3.10)

# Project name
project(remote_sniffer_app)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_path(PCAP_INCLUDE_DIR pcap.h)
find_library(PCAP_LIBRARY pcap)

if (NOT PCAP_INCLUDE_DIR OR NOT PCAP_LIBRARY)
    message(FATAL_ERROR "Could not find libpcap. Please install it or set PCAP_INCLUDE_DIR and PCAP_LIBRARY.")
endif()

find_package(Boost 1.71 REQUIRED COMPONENTS system unit_test_framework)

if (NOT Boost_FOUND)
    message(FATAL_ERROR "Could not find Boost libraries. Please install Boost or set BOOST_ROOT.")
endif()

# Add the executable for the main application
add_executable(remote_sniffer_app remote_sniffer.cpp)

# Include directories
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${PCAP_INCLUDE_DIR})

# Link libraries for the main application
target_link_libraries(remote_sniffer_app ${Boost_LIBRARIES} ${PCAP_LIBRARY} Boost::system)

# Add the executable for unit tests
add_executable(remote_sniffer_app_tests remote_sniffer_tests.cpp)

# Link libraries for the unit tests
target_link_libraries(remote_sniffer_app_tests ${Boost_LIBRARIES} ${PCAP_LIBRARY} Boost::unit_test_framework Boost::system)
