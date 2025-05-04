set(LIB_NAME "TREASUREMAP_INCLUDE")
set(LIB_PATH ${CMAKE_CURRENT_LIST_DIR})

file(GLOB __SRCS
    # ${LIB_PATH}/1/*.cpp
    ${LIB_PATH}/2/*.cpp
)

# include(${LIB_PATH}/Mapobject/mapobject.cmake)
# list(APPEND __SRCS ${LIB_SRCS})

# # message("${LIB_PATH}")
# include(${LIB_PATH}/Game/game.cmake)
# list(APPEND __SRCS ${LIB_SRCS})

# include(${LIB_PATH}/Role/role.cmake)
# list(APPEND __SRCS ${LIB_SRCS})

set(LIB_SRCS ${__SRCS})
