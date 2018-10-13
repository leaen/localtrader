#include "gtest/gtest.h"
#include "client.h"

using namespace exchange;

TEST(ClientTest, can_initialize_with_c_str) {
    Client bob("bob");
    ASSERT_STREQ("bob", bob.get_name().c_str());
}

TEST(ClientTest, can_initialize_with_string) {
    std::string name("bob");
    Client bob(name);
    ASSERT_STREQ("bob", bob.get_name().c_str());
}
