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

bool check_word(const char* word, hashmap_t hashtable[]) {
  for (node *curr = hashtable[hash_function(word)]; curr; curr = curr->next) {
    if (strcmp(curr->word, word) == 0) { return true; }
  }
  return false;
}

int check_words(FILE* fp, hashmap_t hashtable[], char * misspelled[]) {
  for (int i = 0; i < MAX_MISSPELLED; i++) {
    misspelled[i] = NULL;
  }

  int num_misspelled = 0;
  const char *DELIMITERS = " ,.?!/;:\"'\n";
  const int MAX_LINE = 1000;
  char line[MAX_LINE];
  char *token;
  while (fgets(line, MAX_LINE, fp) != NULL) {
    token = strtok(line, DELIMITERS);
    while (token != NULL) {
      if (!check_word(token, hashtable)) {
        misspelled[num_misspelled] = malloc(sizeof(char) * (LENGTH + 1));
        strncpy(misspelled[num_misspelled], token, LENGTH + 1);

        num_misspelled++;
      }
      token = strtok(NULL, DELIMITERS);
    }
  }
  return num_misspelled;
}
