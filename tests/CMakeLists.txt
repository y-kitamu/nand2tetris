# add tests
add_executable(test_vm_translater vm_translater/test_parser.cpp)
target_link_libraries(test_vm_translater gtest_main ${LIBRARIES})
