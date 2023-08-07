
find_package(Boost COMPONENTS unit_test_framework)

if(Boost_FOUND)
    find_program(VALGRIND valgrind)

    include_directories(${Boost_INCLUDE_DIR})

    add_executable(testing
        ValueTests.cpp
        ReaderTests.cpp
        WriterTests.cpp
        PathTests.cpp
    )
    
    target_link_libraries(testing
        library ${Boost_LIBRARIES}
    )

    set_target_properties(testing PROPERTIES OUTPUT_NAME unit)

    add_test(NAME unit-tests COMMAND unit)
    if(VALGRIND)
        add_test(NAME leak-tests COMMAND valgrind --leak-check=summary --error-exitcode=1 ./unit)
    elseif(WIN32)
        add_test(NAME leak-tests COMMAND unit --detect_memory_leaks)
    endif()
else()
    message(STATUS
        "Boost unit test framework is not found; testing targets will not be available")
endif()
