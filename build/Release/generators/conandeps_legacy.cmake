message(STATUS "Conan: Using CMakeDeps conandeps_legacy.cmake aggregator via include()")
message(STATUS "Conan: It is recommended to use explicit find_package() per dependency instead")

find_package(fmt)
find_package(Boost)
find_package(GTest)
find_package(benchmark)

set(CONANDEPS_LEGACY  fmt::fmt  boost::boost  gtest::gtest  benchmark::benchmark_main )