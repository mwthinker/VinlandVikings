#include <gtest/gtest.h>


// Implemented main to make it show up test explorer in Visual Studio 2022.
// Instead of linking to gtest_main or gtest_mock.
int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
