#include <iostream>
#include <assert.h>

struct HNode {
  HNode *next = NULL;
  uint64_t hcode = 0;
};

struct HTab {
  HNode **tab = NULL;
  size_t mask = 0; //n - 1 to to a power of 2 bit comparison
  size_t size = 0;
};

static void h_init(HTab *htab, size_t n) {
  std::cout << "Hash Table Creation" << std::endl;
  std::cout << sizeof(HNode *) << std::endl;
  assert(n > 0 && ((n - 1) & n) == 0);
  htab->tab = (HNode **)calloc(sizeof(HNode *), n);
  htab->mask = n - 1;
  htab->size = 0;
}

static void h_insertion(HTab *htab, HNode *node) {
  std::cout << "Inserting into Hash Table" << std::endl;
  size_t pos = node->hcode & htab->mask;  // slot index
  std::cout << node->hcode << " " << htab->mask << " " << (node->hcode & htab->mask) << std::endl;

  HNode *next = htab->tab[pos];           // prepend the list
  node->next = next;
  htab->tab[pos] = node;
  htab->size++;
}

static HNode **h_lookup(HTab *htab, HNode *key, bool (*eq)(HNode *, HNode *)) {
  std::cout << "Hash Table Lookup" << std::endl;
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
