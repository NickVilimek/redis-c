#include <iostream>
#include <assert.h>
#include "hashtable.h"

void hash_table_init(HTab *htab, size_t n) {
  assert(n > 0 && ((n - 1) & n) == 0);
  htab->tab = (HNode **)calloc(sizeof(HNode *), n);
  htab->mask = n - 1;
  htab->size = 0;
}

void hash_table_insertion(HTab *htab, HNode *node) {
  size_t pos = node->hcode & htab->mask;  // slot index
  HNode *next = htab->tab[pos];           // prepend the list
  node->next = next;
  htab->tab[pos] = node;
  htab->size++;
}

HNode **hash_table_lookup(HTab *htab, HNode *key, bool (*eq)(HNode *, HNode *)) {
  if(!htab->tab) {
    return NULL;
  }

  size_t pos = key->hcode & htab->mask;
  HNode **from = &htab->tab[pos];     // incoming pointer to the result
  for (HNode *cur; (cur = *from) != NULL; from = &cur->next) {
    if (cur->hcode == key->hcode && eq(cur, key)) {
      return from;
    }
  }
  return NULL;
}

HNode* hash_table_detach(HTab *htab, HNode **from) {
  HNode *node = *from;
  *from = node->next;
  htab->size--;
  return node;
}

const size_t k_resizing_work = 128; 

void hm_help_resizing(HMap *hmap) {
  size_t nwork = 0;
  while (nwork < k_resizing_work && hmap->ht2.size > 0) {
    // scan for nodes from ht2 and move them to ht1
    HNode **from = &hmap->ht2.tab[hmap->resizing_pos];
    if (!*from) {
        hmap->resizing_pos++;
        continue;
    }

    hash_table_insertion(&hmap->ht1, hash_table_detach(&hmap->ht2, from));
    nwork++;
  }

  if (hmap->ht2.size == 0 && hmap->ht2.tab) {
    // done
    free(hmap->ht2.tab);
    hmap->ht2 = HTab{};
  }
}

static void hm_start_resizing(HMap *hmap) {
  assert(hmap->ht2.tab == NULL);
  // create a bigger hashtable and swap them
  hmap->ht2 = hmap->ht1;
  hash_table_init(&hmap->ht1, (hmap->ht1.mask + 1) * 2);
  hmap->resizing_pos = 0;
}

HNode *hm_lookup(HMap *hmap, HNode *key, bool (*eq)(HNode *, HNode *)) {
  hm_help_resizing(hmap);
  HNode **from = hash_table_lookup(&hmap->ht1, key, eq);
  from = from ? from : hash_table_lookup(&hmap->ht2, key, eq);
  return from ? *from : NULL;
}

const size_t k_max_load_factor = 8;

void hm_insert(HMap *hmap, HNode *node) {
  if (!hmap->ht1.tab) {
    hash_table_init(&hmap->ht1, 4);
  }

  hash_table_insertion(&hmap->ht1, node);

  if (!hmap->ht2.tab) {
    // check whether we need to resize
    size_t load_factor = hmap->ht1.size / (hmap->ht1.mask + 1);
    if (load_factor >= k_max_load_factor) {
      hm_start_resizing(hmap);
    }
  }

  hm_help_resizing(hmap);
}

HNode *hm_pop(HMap *hmap, HNode *key, bool (*eq)(HNode *, HNode *)) {
  hm_help_resizing(hmap);
  if (HNode **from = hash_table_lookup(&hmap->ht1, key, eq)) {
    return hash_table_detach(&hmap->ht1, from);
  }
  if (HNode **from = hash_table_lookup(&hmap->ht2, key, eq)) {
    return hash_table_detach(&hmap->ht2, from);
  }
  return NULL;
}

size_t hm_size(HMap *hmap) {
  return hmap->ht1.size + hmap->ht2.size;
}

void hm_destroy(HMap *hmap) {
  free(hmap->ht1.tab);
  free(hmap->ht2.tab);
  *hmap = HMap{};
}

void h_scan(HTab *tab, void (*f)(HNode *, void *), void *arg) {
  if (tab->size == 0) {
    return;
  }
  for (size_t i = 0; i < tab->mask + 1; ++i) {
    HNode *node = tab->tab[i];
    while (node) {
      f(node, arg);
      node = node->next;
    }
  }
}
