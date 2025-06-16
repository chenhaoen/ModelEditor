project(VulkanContext CXX)
set(CMAKE_CXX_STANDARD 20)

find_package(Vulkan)
find_package(ktx)

set(VULKANCONTEXT_INCLUDE_DIR ${MODELEDITOR_INCLUDE_DIR}/${PROJECT_NAME})
set(VULKANCONTEXT_SRC_DIR ${MODELEDITOR_SRC_DIR}/${PROJECT_NAME})

file(GLOB VULKANCONTEXT_SOURCES "${VULKANCONTEXT_SRC_DIR}/*.cpp")
file(GLOB VULKANCONTEXT_HEADERS "${VULKANCONTEXT_INCLUDE_DIR}/*.h")

add_library(${PROJECT_NAME} SHARED 
${VULKANCONTEXT_HEADERS}
${VULKANCONTEXT_SOURCES}
)

add_compile_definitions(VULKANCONTEXT_EXPORT)
add_compile_definitions(NOMINMAX)

target_link_libraries(${PROJECT_NAME} PRIVATE
Vulkan::Vulkan
KTX::ktx
)

target_link_libraries(${PROJECT_NAME} PUBLIC
Core
)

if(MSVC)
    # MSVC需要特殊处理
    target_precompile_headers(${PROJECT_NAME} PRIVATE "${VULKANCONTEXT_INCLUDE_DIR}/pch.h")
else()
    target_precompile_headers(${PROJECT_NAME} PRIVATE
        "$<$<COMPILE_LANGUAGE:CXX>:${VULKANCONTEXT_INCLUDE_DIR}/pch.h>"
    )
endif()

install(TARGETS ${PROJECT_NAME}
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

execute_process(COMMAND ${Vulkan_GLSLC_EXECUTABLE} ${CMAKE_CURRENT_LIST_DIR}/shader.vert -o ${CMAKE_CURRENT_LIST_DIR}/vert.spv)
execute_process(COMMAND ${Vulkan_GLSLC_EXECUTABLE} ${CMAKE_CURRENT_LIST_DIR}/shader.frag -o ${CMAKE_CURRENT_LIST_DIR}/frag.spv)
execute_process(COMMAND ${Vulkan_GLSLC_EXECUTABLE} ${CMAKE_CURRENT_LIST_DIR}/skybox.frag -o ${CMAKE_CURRENT_LIST_DIR}/skyboxFrag.spv)
execute_process(COMMAND ${Vulkan_GLSLC_EXECUTABLE} ${CMAKE_CURRENT_LIST_DIR}/skybox.vert -o ${CMAKE_CURRENT_LIST_DIR}/skyboxVert.spv)
execute_process(COMMAND ${Vulkan_GLSLC_EXECUTABLE} ${CMAKE_CURRENT_LIST_DIR}/grids.comp -o ${CMAKE_CURRENT_LIST_DIR}/gridsCompute.spv)
execute_process(COMMAND ${Vulkan_GLSLC_EXECUTABLE} ${CMAKE_CURRENT_LIST_DIR}/grids.vert -o ${CMAKE_CURRENT_LIST_DIR}/gridsVert.spv)
execute_process(COMMAND ${Vulkan_GLSLC_EXECUTABLE} ${CMAKE_CURRENT_LIST_DIR}/grids.frag -o ${CMAKE_CURRENT_LIST_DIR}/gridsFrag.spv)
execute_process(COMMAND ${Vulkan_GLSLC_EXECUTABLE} ${CMAKE_CURRENT_LIST_DIR}/background.vert -o ${CMAKE_CURRENT_LIST_DIR}/backgroundVert.spv)
execute_process(COMMAND ${Vulkan_GLSLC_EXECUTABLE} ${CMAKE_CURRENT_LIST_DIR}/background.frag -o ${CMAKE_CURRENT_LIST_DIR}/backgroundFrag.spv)
execute_process(COMMAND ${Vulkan_GLSLC_EXECUTABLE} ${CMAKE_CURRENT_LIST_DIR}/pbr.vert -o ${CMAKE_CURRENT_LIST_DIR}/pbrVert.spv)
execute_process(COMMAND ${Vulkan_GLSLC_EXECUTABLE} ${CMAKE_CURRENT_LIST_DIR}/pbr.frag -o ${CMAKE_CURRENT_LIST_DIR}/pbrFrag.spv)