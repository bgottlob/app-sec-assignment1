#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dictionary.h"
#include "free_structs.h"

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

    int hash = hash_function(word);

    // If the hash is invalid, don't add this word to the dictionary
    if (hash >= 0 && hash < HASH_SIZE) {
      node *new_node = malloc(sizeof(node));
      strncpy(new_node->word, word, LENGTH + 1);
      new_node->next = NULL;

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
  }
  fclose(fptr);
  return true;
}

void free_hashtable(hashmap_t hashtable[]) {
  for (int i = 0; i < HASH_SIZE; i++) {
    node *curr = hashtable[i];
    while (curr) {
      // Get pointer to next node before current is freed
      node *next = curr->next;

      // Free current node
      curr->next = NULL;
      free(curr);

      curr = next;
    }
  }
}

bool check_word_exact(const char* word, hashmap_t hashtable[]) {
  int hash = hash_function(word);
  // Invalid hash, word can't be in the hashmap
  if (hash >= 0 && hash < HASH_SIZE) {
    for (node *curr = hashtable[hash]; curr; curr = curr->next) {
      if (strcmp(curr->word, word) == 0) { return true; }
    }
  }
  return false;
}

void word_tolower(char lower[]) {
  for (int i = 0; lower[i]; i++) {
    lower[i] = tolower(lower[i]);
  }
}

// Return true if the word has at least one letter
bool has_letter(const char* str) {
  for (int i = 0; str[i] != 0; i++) {
    char lower = tolower(str[i]);
    if (lower >= 'a' && lower <= 'z') { return true; }
  }
  return false;
}

bool check_word(const char* word, hashmap_t hashtable[]) {
  if (strlen(word) > LENGTH) { return false; }
  if (!has_letter(word)) { return true; }
  if (check_word_exact(word, hashtable)) { return true; }

  // If exact match of word is not found, check it lower cased
  char lower[LENGTH + 1];
  strncpy(lower, word, LENGTH + 1);
  word_tolower(lower);
  return check_word_exact(lower, hashtable);
}

int check_words(FILE* fp, hashmap_t hashtable[], char * misspelled[]) {
  for (int i = 0; i < MAX_MISSPELLED; i++) {
    misspelled[i] = NULL;
  }

  int num_misspelled = 0;
  const char *DELIMITERS = " ,.?!/-;:\"'\n";
  const int MAX_LINE = 1000;
  char line[MAX_LINE];
  char *token;
  while (fgets(line, MAX_LINE, fp) != NULL) {
    token = strtok(line, DELIMITERS);
    while (token != NULL) {
      if (!check_word(token, hashtable)) {
        // Preserve original token length when storing it in misspelled list
        unsigned int token_len = strlen(token) + 1;
        misspelled[num_misspelled] = malloc(sizeof(char) * token_len);
        strncpy(misspelled[num_misspelled], token, token_len);

        num_misspelled++;
      }
      token = strtok(NULL, DELIMITERS);
    }
  }
  return num_misspelled;
}

void free_misspelled(char * misspelled[]) {
  for (int i = 0; i < MAX_MISSPELLED; i++) {
    if (misspelled[i]) {
      free(misspelled[i]);
      misspelled[i] = NULL;
    }
  }
}
