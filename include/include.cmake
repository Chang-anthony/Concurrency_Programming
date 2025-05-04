set(LIB_NAME "TREASUREMAP_INCLUDE")
set(LIB_INC_PATH ${CMAKE_CURRENT_LIST_DIR})

set(__INCS
    ${LIB_INC_PATH}/1
    ${LIB_INC_PATH}/2
)

# GLOB 透過cmake 語法 搜尋 path 底下的 *hpp file
# file(GLOB __BIGTWO_INCS
#     "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
#     "${CMAKE_CURRENT_SOURCE_DIR}/include/utils/*.hpp"
# )

set(LIB_INCS ${__INCS})