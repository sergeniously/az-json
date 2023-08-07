
set(SOURCES main.cpp)

add_executable(utility ${SOURCES})

target_link_libraries(utility library)
set_target_properties(utility PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

install(
    TARGETS utility
    DESTINATION ${CMAKE_INSTALL_BINDIR}
)