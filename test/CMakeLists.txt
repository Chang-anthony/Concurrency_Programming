cmake_minimum_required(VERSION 3.13)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "cmake")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}) #set output executable at build file

set(LIB_PATH ${CMAKE_CURRENT_LIST_DIR})

set(CMAKE_CXX_STANDARD 20)

set(GTest Test)

set(Sources 
    ExampleTests.cpp
)

include(${LIB_PATH}/libary/test.cmake)
set(SRCS)

set(INCS ${CMAKE_CURRENT_LIST_DIR}/libary ${CMAKE_CURRENT_LIST_DIR}/libary/FileIterator)
list(APPEND SRCS ${__TEST_UTILS__SRCS})


add_executable(${GTest} ${Sources})

add_library(${GTest}_LIB STATIC
    ${SRCS}
)

if(GTEST_FOUND)
    # include(GoogleTest)
    target_link_libraries(${GTest} PRIVATE ${PROJECT_NAME}_LIB ${GTest}_LIB ${GTest}_LIB GTest::GTest GTest::Main)
    target_include_directories(${GTest}_LIB PUBLIC ${BIGTWO_INCS} ${COMM_INCS} ${INCS})
elseif(NOT GTEST_FOUND)
    target_link_libraries(${GTest} PRIVATE ${PROJECT_NAME}_LIB ${GTest}_LIB ${GTest}_LIB gtest_main)
    target_include_directories(${GTest}_LIB PUBLIC ${BIGTWO_INCS} ${COMM_INCS} ${INCS})
endif()

# target_include_directories(${PROJECT_NAME}_LIB PUBLIC ${BIGTWO_INCS} ${COMM_INCS})
# target_link_libraries(${GTest}_LIB PRIVATE ${PROJECT_NAME}_LIB)

# 指定生成的可执行文件路径
# set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Run)

include(GoogleTest)
gtest_discover_tests(${GTest})


add_test(
    NAME ${GTest}
    COMMAND ${GTest}
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)

set(GTESTON ON)
# set(GTESTOFF OFF)
# 在主程序之前添加一个自定义目标以运行测试
if(GTESTON)
    # # Set the working directory for the test
    # set_target_properties(${GTEST} PROPERTIES
    # RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    # )
    # add_custom_target(RunTests
    # COMMAND ${CMAKE_CTEST_COMMAND} -V
    # WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    # COMMENT "Running tests before the main program"
    # )
    # # 添加依赖关系，确保在运行主程序之前运行测试
    # add_dependencies(${PROJECT_NAME} RunTests)

    # 添加自定义命令，该命令在构建完成后运行
    # add_custom_command(
    #     TARGET ${GTest}
    #     POST_BUILD
    #     COMMAND ${CMAKE_COMMAND} -E echo "Build completed. Running execute_process..."
    #     COMMAND ${CMAKE_COMMAND} -E echo "Your additional commands here..."
    #     COMMAND ${CMAKE_CURRENT_LIST_DIR}/run_tests.bat
    # )

    # 檢測系統是否為 macOS
    if(APPLE OR UNIX)
        # 在 macOS 上執行 bash 腳本
        # # 添加自定义目标，该目标依赖于构建完成后运行的自定义命令
        # add_custom_command(
        #     TARGET ${GTest}
        #     POST_BUILD #等待executable build 玩才跑以下指令
        #     COMMAND ${CMAKE_CURRENT_LIST_DIR}/run_tests.sh
        #     COMMENT "Running run_tests"
        # )
        # # 定義一個自定義目標，用來執行你的自定義命令
        # add_custom_target(RunTests DEPENDS ${GTest})

        add_custom_target(RunTests
            COMMAND ${CMAKE_COMMAND} -E echo "Build completed. Running execute_process..."
            COMMAND ${CMAKE_COMMAND} -E echo "Your additional commands here..."
            COMMAND ${CMAKE_CURRENT_LIST_DIR}/run_tests.sh
            DEPENDS ${GTest}
            # COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/test/Run/DesignPattern_Test.exe
        )
        add_dependencies(${PROJECT_NAME} RunTests)
    endif()

    if(WIN32)
        message(STATUS "Run Test")
        # add_custom_command(
        #     TARGET ${GTest}
        #     POST_BUILD
        #     COMMAND ${CMAKE_CURRENT_LIST_DIR}/run_tests.bat
        #     COMMENT "Running run_tests"
        # )
        # add_custom_target(RunTests DEPENDS ${GTest})

        #TODO: need to know why this can run shell file when build Almost completed
        # # 添加自定义目标，该目标依赖于构建完成后运行的自定义命令
        add_custom_target(RunTests
            COMMAND ${CMAKE_COMMAND} -E echo "Build completed. Running execute_process..."
            COMMAND ${CMAKE_COMMAND} -E echo "Your additional commands here..."
            COMMAND ${CMAKE_CURRENT_LIST_DIR}/run_tests.bat
            DEPENDS ${GTest}
        )
        add_dependencies(${PROJECT_NAME} RunTests)
    endif()

endif()





