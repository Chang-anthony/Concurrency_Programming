#include <iostream>
#include <gtest/gtest.h>
#include <gtest/gtest-spi.h>


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    
    return RUN_ALL_TESTS();
}

