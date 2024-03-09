
#include <gtest/gtest.h>
#include <iostream>

#include "../src/helpers/hash.h"

using namespace std;

TEST(HashingFunction, hash) {
  string key = "hello";

  uint64_t out = jenkins_one_at_a_time_hash((uint8_t *) key.data(), key.size());

  EXPECT_EQ(out, 1759170473);
}

TEST(HashingFunction, hash_null_string) {
  string key;

  uint64_t out = jenkins_one_at_a_time_hash((uint8_t *) key.data(), key.size());

  EXPECT_EQ(out, 2166136261);
}