#ifndef LITH_LIST_H
#define LITH_LIST_H

#include <stddef.h>

#define Lith_ForList(var, name) \
   for(list_t *rover = (name).next; rover != &(name); rover = rover->next) \
      __with(var = rover->object;)

#define Lith_ForListIter(var, name, ...) \
   for(list_t *rover = (name).next; rover != &(name); rover = rover->next, __VA_ARGS__) \
      __with(var = rover->object;)


//----------------------------------------------------------------------------
// Type Definitions
//

// To clarify how these new linked list structures work:
// You put a list_t in your structure (or anywhere, you could allocate one)
// and run LinkDefault on it to construct it proper.
//
// You can then have a head node which you iterate over. The head itself is
// a dummy node with no object, but you can iterate over it bidirectionally,
// as the back is head->prev and the front is head->next.
//
// This removes any real need to reverse a linked list, and you can insert
// backwards. Just be careful that all the nodes get constructed correctly.
//
// You can delete an entire list (destroying objects with a custom function or
// not at all) with ListFree, and get the size of a headed list with ListSize.

typedef struct list_s
{
   void *object;
   struct list_s *prev, *next;
} list_t;


//----------------------------------------------------------------------------
// Extern Functions
//

[[__optional_args(1)]] void Lith_LinkDefault(list_t *list, void *object);
void Lith_ListLink(list_t *head, list_t *list);
void *Lith_ListUnlink(list_t *list);
size_t Lith_ListSize(list_t *head);
[[__optional_args(1)]] void Lith_ListFree(list_t *head, void (*deleter)(void *));

#endif
