#pragma once

#include <stddef.h>
#include <stdint.h>

struct HNode {
  HNode *next = NULL;
  uint64_t hcode = 0;
};

struct HTab {
  HNode **tab = NULL;
  size_t mask = 0; //n - 1 to to a power of 2 bit comparison
  size_t size = 0;
};

struct HMap {
    HTab ht1;   // newer
    HTab ht2;   // older
    size_t resizing_pos = 0;
};

void hash_table_init(HTab *htab, size_t n);
void hash_table_insertion(HTab *htab, HNode *node);
HNode **hash_table_lookup(HTab *htab, HNode *key, bool (*eq)(HNode *, HNode *));
HNode* hash_table_detach(HTab *htab, HNode **from);

HNode *hm_lookup(HMap *hmap, HNode *key, bool (*eq)(HNode *, HNode *));
void hm_insert(HMap *hmap, HNode *node);
HNode *hm_pop(HMap *hmap, HNode *key, bool (*eq)(HNode *, HNode *));
size_t hm_size(HMap *hmap);
void hm_destroy(HMap *hmap);