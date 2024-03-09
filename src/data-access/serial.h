#pragma once

#include <iostream>

/* Data Types */
enum {
  SER_NIL = 0,    // Like `NULL`
  SER_ERR = 1,    // An error code and message
  SER_STR = 2,    // A string
  SER_INT = 3,    // A int64
  SER_ARR = 4,    // Array
};

void out_nil(std::string &out) {
  out.push_back(SER_NIL);
}

void out_str(std::string &out, const std::string &val) {
  out.push_back(SER_STR);
  uint32_t len = (uint32_t)val.size();
  out.append((char *)&len, 4);
  out.append(val);
}

static void out_int(std::string &out, int64_t val) {
  out.push_back(SER_INT);
  out.append((char *)&val, 8);
}

static void out_err(std::string &out, int32_t code, const std::string &msg) {
  out.push_back(SER_ERR);
  out.append((char *)&code, 4);
  uint32_t len = (uint32_t)msg.size();
  out.append((char *)&len, 4);
  out.append(msg);
}

static void out_arr(std::string &out, uint32_t n) {
  out.push_back(SER_ARR);
  out.append((char *)&n, 4);
}