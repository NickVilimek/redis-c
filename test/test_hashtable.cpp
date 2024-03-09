// test/test_sample.cpp
#include <gtest/gtest.h>
#include <iostream>
#include "../src/ds/hashtable.h"

using namespace std;

#define container_of(ptr, type, member) ({                  \
    const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
    (type *)( (char *)__mptr - offsetof(type, member) );})

HTab hash_table;

static bool entry_eq(HNode *lhs, HNode *rhs) {
  bool flag = lhs->hcode == rhs->hcode;
  return flag;
}

TEST(HashTable, hash_table_init) {
  size_t table_size = 4;
  hash_table_init(&hash_table, table_size);

  EXPECT_EQ(hash_table.mask, table_size - 1);
  EXPECT_EQ(hash_table.size, 0);
}

TEST(HashTable, insert_and_lookup) {

  hash_table_init(&hash_table, (size_t) 4);

  uint64_t hello_string_hash = 2166136261;

  HNode *node = (HNode*) malloc(sizeof(HNode));
  node->hcode = hello_string_hash;

  hash_table_insertion(&hash_table, node);

  HNode **lookup = hash_table_lookup(&hash_table, node, &entry_eq);

  EXPECT_EQ((*lookup)->hcode, 2166136261);
  EXPECT_EQ((*lookup), node);

  free(node);
}

TEST(HashTable, hash_table_detach) {

  hash_table_init(&hash_table, (size_t) 4);

  uint64_t hello_string_hash = 2166136261;
  vector<HNode *> nodes;

  HNode* toDelete;

  for(size_t i = 0; i < 5; i++) {
    HNode* node = (HNode *) malloc(sizeof(HNode));

    if(i == 3) {
      node->hcode = (uint64_t) 2166136261;
      toDelete = node;
    } else {
      node->hcode = (uint64_t) 2;
    }

    hash_table_insertion(&hash_table, node);

    nodes.push_back(node);
  }

  HNode **from= hash_table_lookup(&hash_table, toDelete, &entry_eq);

  hash_table_detach(&hash_table, from);

  HNode **lookup = hash_table_lookup(&hash_table, toDelete, &entry_eq);

  EXPECT_EQ(lookup == nullptr, true);

  // Cleanuo
  for(size_t i = 0; i < 5; i++) {
    HNode* node = nodes[i];
    free(node);
    nodes[i] = NULL;
  }
}