project(Editor CXX)
set(CMAKE_CXX_STANDARD 20)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

qt_standard_project_setup()

set(EDITOR_INCLUDE_DIR ${MODELEDITOR_INCLUDE_DIR}/${PROJECT_NAME})
set(EDITOR_SRC_DIR ${MODELEDITOR_SRC_DIR}/${PROJECT_NAME})

file(GLOB EDITOR_SOURCES "${EDITOR_SRC_DIR}/*.cpp")
file(GLOB EDITOR_HEADERS "${EDITOR_INCLUDE_DIR}/*.h")
file(GLOB EDITOR_UIS "${EDITOR_SRC_DIR}/*.ui")

qt_add_executable(${PROJECT_NAME}
${EDITOR_SOURCES}
${EDITOR_HEADERS}
${EDITOR_UIS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${Qt6Core_LIBRARIES}
    ${Qt6Gui_LIBRARIES}
    ${Qt6Widgets_LIBRARIES}
    VulkanContext
    IO
)

if(MSVC)
    # MSVC需要特殊处理
    target_precompile_headers(${PROJECT_NAME} PRIVATE "${EDITOR_INCLUDE_DIR}/pch.h")
else()
    target_precompile_headers(${PROJECT_NAME} PRIVATE
        "$<$<COMPILE_LANGUAGE:CXX>:${EDITOR_INCLUDE_DIR}/pch.h>"
    )
endif()

install(TARGETS ${PROJECT_NAME}
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)