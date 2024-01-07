set(MODULE_NAME c++20.modules.consumer)

add_executable(${MODULE_NAME})

target_sources(
  ${MODULE_NAME}
  PRIVATE
    main.cpp
)

gn3_target_link_binary_to_src(${MODULE_NAME})

target_link_libraries(${MODULE_NAME} PRIVATE c++20.modules)
