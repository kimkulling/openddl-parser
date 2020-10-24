#include "gtest/gtest.h"

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);

    // seed the randomizer with the current system time
    time_t t;
    time(&t);
    srand((unsigned int)t);

    int result = RUN_ALL_TESTS();

    return result;
}
