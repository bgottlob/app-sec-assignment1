// Defines functions for freeing misspelled word lists and dictionary hashtables
#ifndef FREE_STRUCTS_H
#define FREE_STRUCTS_H

#include "dictionary.h"

void free_hashtable(hashmap_t hashtable[]);
void free_misspelled(char * misspelled[]);

#endif // FREE_STRUCTS_H
