project(Core CXX)
set(CMAKE_CXX_STANDARD 20)

find_package(glm)

set(CORE_INCLUDE_DIR ${MODELEDITOR_INCLUDE_DIR}/${PROJECT_NAME})
set(CORE_SRC_DIR ${MODELEDITOR_SRC_DIR}/${PROJECT_NAME})

file(GLOB CORE_SOURCES "${CORE_SRC_DIR}/*.cpp" "${CORE_INCLUDE_DIR}/*.h")

add_compile_definitions(CORE_EXPORT)

add_library(${PROJECT_NAME} SHARED 
${CORE_SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    glm::glm
)

install(TARGETS ${PROJECT_NAME}
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

add_subdirectory(Pipeline)
add_subdirectory(Commands)