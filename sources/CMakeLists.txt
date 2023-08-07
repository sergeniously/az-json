
set(SOURCES
    Error.cpp
    Value.cpp
    Path.cpp
    Reader.cpp
    Writer.cpp
)

add_library(library STATIC ${SOURCES})

set_target_properties(library PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

install(
    TARGETS library
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
