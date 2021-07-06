#==============================
# Requirements
#==============================

cmake_minimum_required(VERSION 3.10)

#==============================
# Project Settings
#==============================

if (NOT PROJECT_DECLARED)
    project("Gaia Shared Picture" LANGUAGES CXX VERSION 0.9)
    set(PROJECT_DECLARED)
endif()

#==============================
# Project Modules
#==============================

add_subdirectory("GaiaSharedPicture")

if (WITH_TEST)
    add_subdirectory("TestWriter")
    add_subdirectory("TestReader")
endif()