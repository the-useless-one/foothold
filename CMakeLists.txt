cmake_minimum_required(VERSION 3.0)

project(foothold)

include_directories(inc)

file(
    GLOB_RECURSE

    source_files

    src/*

    inc/*
)

add_executable(
    foothold

    ${source_files}
)

