cmake_minimum_required(VERSION 3.12)

project(shannon_fano
    VERSION 1.0.0
    DESCRIPTION "Portable Shannon-Fano coding example"
    LANGUAGES CXX
)

add_executable(shannon_fano src/shannon_fano.cpp)

target_compile_features(shannon_fano PRIVATE cxx_std_17)

if (MSVC)
    target_compile_options(shannon_fano PRIVATE /W4)
else()
    target_compile_options(shannon_fano PRIVATE -Wall -Wextra -pedantic)
endif()
