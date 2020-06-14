#include <stdlib.h>
#include <stdio.h>
#include "dictionary.h"

int main(int argc, char **argv) {
  const char *corpus = NULL;
  const char *dictionary = NULL;
  if (argc != 3) {
    printf("Usage: spell_check <corpus_file> <dictionary_file>\n");
    exit(EXIT_FAILURE);
  } else {
    corpus = argv[1];
    dictionary = argv[2];
    printf("Checking `%s` against corpus `%s`\n", dictionary, corpus);
    FILE *corpusfp = fopen(corpus, "r");
    if (corpusfp == NULL) {
      printf("Unable to open file `%s`\n", corpus);
      exit(EXIT_FAILURE);
    }

    node *hashtable[HASH_SIZE];
    load_dictionary(dictionary, hashtable);
    char *misspelled[MAX_MISSPELLED];
    int numMisspelled = check_words(corpusfp, hashtable, misspelled);
    fclose(corpusfp);

    printf("%d words misspelled in `%s`\n", numMisspelled, corpus);
  }
}
