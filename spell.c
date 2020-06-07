#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dictionary.h"

bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[]) {
  FILE *fptr = fopen(dictionary_file, "r");
  if (fptr == NULL) {
    printf("Unable to open file `%s`\n", dictionary_file);
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < HASH_SIZE; i++) {
    hashtable[i] = NULL;
  }

  char word[LENGTH + 1];
  while (fgets(word, sizeof(word), fptr) != NULL) {
    word[strcspn(word, "\n")] = 0; // Remove trailing newline

    node *new_node = malloc(sizeof(node));
    strncpy(new_node->word, word, LENGTH + 1);
    new_node->next = NULL;

    int hash = hash_function(word);
    node *list = hashtable[hash];

    node *curr = list;
    if (curr) { // Traverse to the end of the list then append current word
      while (curr->next) { curr = curr->next; }
      curr->next = new_node;
      hashtable[hash] = list;
    } else { // This word is the first in the hashmap list
      hashtable[hash] = new_node;
    }
  }
  fclose(fptr);
  return true;
}
