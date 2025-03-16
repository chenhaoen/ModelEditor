project(IO CXX)
set(CMAKE_CXX_STANDARD 20)

find_package(assimp)
find_package(glm)

set(IO_INCLUDE_DIR ${MODELEDITOR_INCLUDE_DIR}/${PROJECT_NAME})
set(IO_SRC_DIR ${MODELEDITOR_SRC_DIR}/${PROJECT_NAME})

file(GLOB IO_SOURCES "${IO_SRC_DIR}/*.cpp")
file(GLOB IO_HEADERS "${IO_INCLUDE_DIR}/*.h")

add_compile_definitions(IO_EXPORT)
add_compile_definitions(STB_IMAGE_IMPLEMENTATION)

add_library(${PROJECT_NAME} SHARED 
${IO_SOURCES}
${IO_HEADERS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
   Core
   glm::glm
   assimp::assimp
)

install(TARGETS ${PROJECT_NAME}
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)