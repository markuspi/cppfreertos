
file(GLOB_RECURSE sources "../src/*.*")
idf_component_register(SRCS "main.cpp" ${sources})
