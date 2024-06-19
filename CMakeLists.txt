cmake_minimum_required(VERSION 3.12)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "cmake")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})
project(CONCURRENCYPROGRAMMING)
set(CMAKE_CXX_STANDARD 20)

if(${CMAKE_C_COMPILER_ID} STREQUAL "AppleClang")
    set(CMAKE_OSX_DEPLOYMENT_TARGET "14.1" CACHE STRING "Minimum OS X deployment version" FORCE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-command-line-argument")
endif()

if(CMAKE_CXX_COMPILE_ID STREQUAL "AppleClang")
    set(CMAKE_OSX_DEPLOYMENT_TARGET "14.1" CACHE STRING "Minimum OS X deployment version" FORCE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-command-line-argument")
endif()

# ============ set

set(INCLUDE_PATH ${CMAKE_CURRENT_LIST_DIR}/include)
set(SRC_PATH ${CMAKE_CURRENT_LIST_DIR}/src)
set(UTILS_PATH ${CMAKE_CURRENT_LIST_DIR}/include/utils)
set(TEST_UTILS_PATH ${CMAKE_CURRENT_LIST_DIR}/test/libary)

# ============

# ========================================================
# GTEST
find_package(GTEST REQUIRED)

if(NOT GTEST_FOUND)
    message(STATUS "Start Fetch Google Test.")
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
        # DOWNLOAD_NO_EXTRACT TRUE
        # GIT_TAG        release-1.11.0
        DOWNLOAD_EXTRACT_TIMESTAMP ON
    )

    # set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    # set_property(TARGET gmock PROPERTY FOLDER "gtest")
    # set_property(TARGET gmock_main PROPERTY FOLDER "gtest")
    # set_property(TARGET gtest PROPERTY FOLDER "gtest")
    # set_property(TARGET gtest_main PROPERTY FOLDER "gtest")
endif()

# # 確認是否已經包含 Google Test
# if(TARGET gtest)
#     message(STATUS "Google Test found!")
# else()
#     message(FATAL_ERROR "Failed to find Google Test!")
# endif()

# ======================================================


# ================= main
include(${INCLUDE_PATH}/include.cmake)
include(${TEST_UTILS_PATH}/test.cmake)
include(${SRC_PATH}/source.cmake)

set(SRCS)
set(INCS)
list(APPEND SRCS ${LIB_SRCS})
list(APPEND INCS ${LIB_INCS})

include(${UTILS_PATH}/utils.cmake)
set(COMM_INCS ${UTILS_INCS} ${__TEST_UTILS_INCS})
set(COMM_SRCS ${__TEST_UTILS__SRCS})
# ==================


add_library(${PROJECT_NAME}_LIB STATIC
    ${SRCS}
    ${COMM_SRCS}
)


# 指定項目的執行檔名稱和源檔
# 將所有源檔案放入變數中
set(SOURCES
    main.cpp
)
add_executable(${PROJECT_NAME} ${SOURCES})

# 將項目的 include 路徑設定為當前目錄（或是你的標頭檔所在的目錄）
# target_include_directories(${PROJECT_NAME} PUBLIC ${BIGTWO_INCS} ${COMM_INCS})
# 将包含目录添加为库的接口目录
# PRIVATE main main檔連接到這個庫的時候會找不到 hpp 檔 PUBLIC 才行
target_include_directories(${PROJECT_NAME}_LIB PUBLIC ${INCS} ${COMM_INCS})
# target_include_directories(${PROJECT_NAME}_LIB PUBLIC ${INCLUDE_PATH}/Card ${INCLUDE_PATH}/Player ${UTILS_PATH})


# 链接库到可执行文件
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_LIB)

# 設定項目的目標特性，比如 C++ 版本
# target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_compile_options(${PROJECT_NAME} PRIVATE -finput-charset=UTF-8)

# 設置MSVC編譯編碼
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8>")
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")


target_compile_options(${PROJECT_NAME} PRIVATE 
    -Wall -Werror -Wno-unused-parameter -Wno-unused-function -Wno-unused-variable
    -fdata-sections -ffunction-sections -fno-strict-aliasing -fno-inline -fno-common
    -Wmissing-declarations -Wpointer-arith -Wshadow -Wfloat-equal -MD -MP
    -g -fstack-usage
)



#GTEST
enable_testing()
add_subdirectory(test)


# 在主程序之前添加一个自定义目标以运行测试
# add_custom_target(RunTests
#     COMMAND ${CMAKE_CTEST_COMMAND}
#     WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
#     COMMENT "Running tests before the main program"
# )


# 添加依赖关系，确保在运行主程序之前运行测试
# add_dependencies(${PROJECT_NAME} RunTests)

