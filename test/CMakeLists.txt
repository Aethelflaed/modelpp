# This function takes a file in argument and create a new executable
# using this file.
#
# The executable created is also added to the test list, and can simply be run
# by calling `make test`
function(create_test file)
  get_filename_component(test_name ${file} NAME_WE)

  # Append /test to the output directory
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/test)

  add_executable(${test_name} ${file})
  target_link_libraries(${test_name}
    # Link to boost test framework
    boost_unit_test_framework
    )

  add_test(
    NAME ${test_name}
    COMMAND ${test_name}
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}
    )

  if (LCOV_PATH)
    setup_target_for_coverage_lcov(
      NAME coverage_${test_name}
      EXECUTABLE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/${test_name}
      )
  endif()

  set(file_to_binary_info
    "${CMAKE_CURRENT_SOURCE_DIR}/${file}:${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/${test_name}")
  set(TEST_FILE_TO_BINARY ${TEST_FILE_TO_BINARY} ${file_to_binary_info} PARENT_SCOPE)
endfunction()

create_test(model_test.cpp)
create_test(inherited_model_test.cpp)

list(JOIN TEST_FILE_TO_BINARY "\n" TEST_FILE_TO_BINARY)
file(WRITE "${CMAKE_BINARY_DIR}/file_to_binary" "${TEST_FILE_TO_BINARY}\n")
