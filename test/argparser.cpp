#include <gtest/gtest.h>

extern "C" {
#include "main-mock.h"
}

#include <array>

TEST(argparser, test) {
    auto args = std::to_array({"./test_program", "test", "-f", "11"});
    ASSERT_EQ(mock_main(args.size(), const_cast<char **>(args.data())), 0);
}

TEST(argparser, parse_array_of_string) {
    auto args = std::to_array({"./test_program"});
    ASSERT_EQ(test_parse_array_of_strings(args.size(), const_cast<char **>(args.data())), 0);
}

TEST(argparser, action_store_type_uint) {
    auto args = std::to_array({"./test_program"});
    ASSERT_EQ(test_action_store_type_uint(args.size(), const_cast<char **>(args.data())), 0);
}

TEST(argparser, action_store_true) {
    auto args = std::to_array({"./test_program"});
    ASSERT_EQ(test_action_store_true(args.size(), const_cast<char **>(args.data())), 0);
}

TEST(argparser, action_store_false) {
    auto args = std::to_array({"./test_program"});
    ASSERT_EQ(test_action_store_false(args.size(), const_cast<char **>(args.data())), 0);
}

TEST(argparser, optional_args_are_position_independent) {
    auto args = std::to_array({"./test_program"});
    ASSERT_EQ(test_optional_args_are_position_independent(args.size(), const_cast<char **>(args.data())), 0);
}
