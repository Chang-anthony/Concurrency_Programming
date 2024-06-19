set(__NAME "test_interface")
set(__LIB_PATH ${CMAKE_CURRENT_LIST_DIR})

set(__TEST_UTILS__SRCS 
    ${__LIB_PATH}/FileIterator/FileLine.cpp
    ${__LIB_PATH}/FileIterator/FileIterator.cpp
)

set(__TEST_UTILS_INCS
    ${__LIB_PATH}
    ${__LIB_PATH}/FileIterator

)

