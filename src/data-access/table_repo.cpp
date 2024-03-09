
#include <unistd.h>
#include <cstdint>
#include <iostream>
#include "hashtable.h"
#include "../helpers/hash.h"
#include "table_repo.h"
#include <cassert>
#include "serial.h"

/* Composable Functions */

void cb_scan(HNode *node, void *arg) {
  std::string &out = *(std::string *)arg;
  out_str(out, container_of(node, Entry, node)->key);
}

bool entry_eq(HNode *lhs, HNode *rhs) {
  struct Entry *le = container_of(lhs, struct Entry, node);
  struct Entry *re = container_of(lhs, struct Entry, node);
  return le->key == re->key;
}

/* Helper Functions */

void gen_too_big_err(std::string &out) {
  out_err(out, ERR_2BIG, "response is too big");
}

void gen_unknown_cmd_err(std::string &out) {
  out_err(out, ERR_UNKNOWN, "Unkown command");
}

/* Access Functions */

void do_get(HMap *hmap, std::vector<std::string> &cmd, std::string &out) {
  Entry key;
  key.key.swap(cmd[1]);
  key.node.hcode = jenkins_one_at_a_time_hash((uint8_t *) key.key.data(), key.key.size());

  HNode *node = hm_lookup(hmap, &key.node, &entry_eq);
  if(!node) {
    return out_nil(out);
  }

  const std::string &val = container_of(node, Entry, node)->val;
  out_str(out, val);
}

void do_set(HMap *hmap, std::vector<std::string> &cmd, std::string &out) {
  Entry key;
  key.key.swap(cmd[1]);
  key.node.hcode = jenkins_one_at_a_time_hash((uint8_t *) key.key.data(), key.key.size());

  HNode *node = hm_lookup(hmap, &key.node, &entry_eq);
  if(node) {
    container_of(node, Entry, node)->val.swap(cmd[2]);
  } else {
    Entry *ent = new Entry();
    ent->key.swap(key.key);
    ent->node.hcode = key.node.hcode;
    ent->val.swap(cmd[2]);
    hm_insert(hmap, &ent->node);
  }

  return out_nil(out);
}

void do_del(HMap *hmap, std::vector<std::string> &cmd, std::string &out) {
  Entry key;
  key.key.swap(cmd[1]);
  key.node.hcode = jenkins_one_at_a_time_hash((uint8_t *) key.key.data(), key.key.size());

  HNode *node = hm_pop(hmap, &key.node, &entry_eq);
  if(node) {
    delete container_of(node, Entry, node);
  } 

  return out_int(out, node ? 1 : 0);
}


void do_keys(HMap *hmap, std::vector<std::string> &cmd, std::string &out) {
  (void)cmd;
  out_arr(out, (uint32_t) hm_size(hmap));
  h_scan(&hmap->ht1, &cb_scan, &out);
  h_scan(&hmap->ht2, &cb_scan, &out);
}