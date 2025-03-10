project(Nodes CXX)
set(CMAKE_CXX_STANDARD 20)

set(NODES_INCLUDE_DIR ${MODELEDITOR_INCLUDE_DIR}/${PROJECT_NAME})
set(NODES_SRC_DIR ${MODELEDITOR_SRC_DIR}/${PROJECT_NAME})

file(GLOB NODES_SOURCES "${NODES_SRC_DIR}/*.cpp")
file(GLOB NODEST_HEADERS "${NODES_INCLUDE_DIR}/*.h")

add_compile_definitions(NODES_EXPORT)

add_library(${PROJECT_NAME} SHARED 
${NODEST_HEADERS}
${NODES_SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    RenderingContextDriver
)

install(TARGETS ${PROJECT_NAME}
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)