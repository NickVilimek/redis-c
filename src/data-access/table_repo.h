#pragma once

#include <unistd.h>
#include <cstdint>
#include <iostream>
#include "hashtable.h"

enum {
  RES_OK = 0,
  RES_ERR = 1,
  RES_NX = 2,
};

enum {
  ERR_UNKNOWN = 1,
  ERR_2BIG = 2,
};

#define container_of(ptr, type, member) ({                  \
    const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
    (type *)( (char *)__mptr - offsetof(type, member) );})

struct Entry {
  struct HNode node;
  std::string key;
  std::string val;
};

bool entry_eq(HNode *lhs, HNode *rhs);
void cb_scan(HNode *node, void *arg);

void gen_too_big_err(std::string &out);
void gen_unknown_cmd_err(std::string &out);

void do_get(HMap *hmap, std::vector<std::string> &cmd, std::string &out);
void do_set(HMap *hmap, std::vector<std::string> &cmd, std::string &out);
void do_del(HMap *hmap, std::vector<std::string> &cmd, std::string &out);
void do_keys(HMap *hmap, std::vector<std::string> &cmd, std::string &out);