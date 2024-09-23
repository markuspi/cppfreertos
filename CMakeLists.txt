cmake_minimum_required(VERSION 3.15)

set(REQUIRED_COMPONENTS "esp_ringbuf")

if(DEFINED DEV_ENVIRONMENT)
    set(COMPONENTS "main" "__pio_env" ${REQUIRED_COMPONENTS})
    include($ENV{IDF_PATH}/tools/cmake/project.cmake)
    project(cppfreertos)
else()
    idf_component_register(SRC_DIRS "src" INCLUDE_DIRS "include" REQUIRES ${REQUIRED_COMPONENTS})
endif()
