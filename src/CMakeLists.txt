set(SRC
  "./main.cpp"
)
add_executable("${PROJECT_NAME}" "${SRC}")

target_link_directories("${PROJECT_NAME}" PRIVATE
  "${CMAKE_SOURCE_DIR}/lib"
)
