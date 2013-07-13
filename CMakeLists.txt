cmake_minimum_required(VERSION 2.8)
project(abstract_factory)

find_package(Boost COMPONENTS unit_test_framework log REQUIRED)

set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++")
add_definitions(-DBOOST_ALL_DYN_LINK)

enable_testing()
add_executable(tst_abstract_factory src/testing/tst_abstract_factory.cpp)
target_link_libraries(tst_abstract_factory ${Boost_LIBRARIES})
add_test(abstract_factory_test tst_abstract_factory)