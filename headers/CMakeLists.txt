
file(GLOB HEADERS "az/json/*.h")

install(
    FILES ${HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/az/json
)
