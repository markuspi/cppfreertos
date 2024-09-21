cmake_minimum_required(VERSION 3.15)

if(ESP_PLATFORM)
    idf_component_register(SRC_DIRS "src" INCLUDE_DIRS "include" REQUIRES freertos)
    return()
endif()

project(cppfreertos VERSION 0.1.0)
