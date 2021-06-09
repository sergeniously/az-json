
set(SOURCES
    Error.cpp
    Value.cpp
    Path.cpp
    Reader.cpp
    Writer.cpp
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})

install(
    TARGETS ${PROJECT_NAME}
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
)