project(Core CXX)
set(CMAKE_CXX_STANDARD 20)

find_package(glm)

set(CORE_INCLUDE_DIR ${MODELEDITOR_INCLUDE_DIR}/${PROJECT_NAME})
set(CORE_SRC_DIR ${MODELEDITOR_SRC_DIR}/${PROJECT_NAME})

file(GLOB CORE_SOURCES "${CORE_SRC_DIR}/*.cpp")
file(GLOB CORE_HEADERS "${CORE_INCLUDE_DIR}/*.h")

add_compile_definitions(CORE_EXPORT)

add_library(${PROJECT_NAME} SHARED 
${CORE_SOURCES}
${CORE_HEADERS}
)

if(MSVC)
    # MSVC��Ҫ���⴦��
    target_precompile_headers(${PROJECT_NAME} PRIVATE "${CORE_INCLUDE_DIR}/pch.h")
else()
    target_precompile_headers(${PROJECT_NAME} PRIVATE
        "$<$<COMPILE_LANGUAGE:CXX>:${CORE_INCLUDE_DIR}/pch.h>"
    )
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE
    glm::glm
)

install(TARGETS ${PROJECT_NAME}
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

function(add_childdirectory child)
    add_subdirectory(${child})

    set(INCLUDE_DIR ${MODELEDITOR_INCLUDE_DIR}/${PROJECT_NAME}/${child})
    set(SRC_DIR ${MODELEDITOR_SRC_DIR}/${PROJECT_NAME}/${child})

    file(GLOB INCLUDE_FILES "${INCLUDE_DIR}/*.h")
    file(GLOB SRC_FILES "${SRC_DIR}/*.cpp")

    source_group("Header Files\\${child}" FILES ${INCLUDE_FILES})
    source_group("Source Files\\${child}" FILES ${SRC_FILES})
endfunction()


add_childdirectory(Pipeline)
add_childdirectory(Commands)
add_childdirectory(RenderingContextDriver)
add_childdirectory(Shader)