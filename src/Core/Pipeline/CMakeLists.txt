set(RCD_SRC_DIR ${MODELEDITOR_SRC_DIR}/${PROJECT_NAME}/RenderingContextDriver)
set(RCD_INCLUDE_DIR ${MODELEDITOR_INCLUDE_DIR}/${PROJECT_NAME}/RenderingContextDriver)

file(GLOB RCD_INCLUDE_FILES "${RCD_INCLUDE_DIR}/*.h")
source_group("Header Files\\RenderingContextDriver" FILES ${RCD_INCLUDE_FILES})

file(GLOB RCD_SRC_FILES "${RCD_SRC_DIR}/*.cpp")
source_group("Source Files\\RenderingContextDriver" FILES ${RCD_SRC_FILES})

target_sources(Core 
  PRIVATE 
    ${RCD_INCLUDE_FILES}
    ${RCD_SRC_FILES}
)