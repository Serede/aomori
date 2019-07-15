add_library(aomori aomori.c)

add_executable(aomori-main main.c)
target_link_libraries(aomori-main aomori)
set_target_properties(aomori-main PROPERTIES OUTPUT_NAME aomori)

add_subdirectory(test)
