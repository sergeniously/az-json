
find_package(Boost REQUIRED COMPONENTS unit_test_framework)
find_program(VALGRIND valgrind)

add_executable(${PROJECT_NAME}-test
    ValueTests.cpp
    ReaderTests.cpp
    WriterTests.cpp
    PathTests.cpp
)
add_dependencies(${PROJECT_NAME}-test ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}-test
    ${PROJECT_NAME} ${Boost_LIBRARIES}
)

add_test(NAME unit-tests COMMAND ${PROJECT_NAME}-test)
if(VALGRIND)
    add_test(NAME leak-tests COMMAND valgrind --leak-check=summary --error-exitcode=1 ./${PROJECT_NAME}-test)
endif()
