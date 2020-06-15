#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include "../dictionary.h"
#include "test_suites.h"

int count_lines(const char* dictionary_file) {
  FILE *f = fopen(dictionary_file, "r");
  if (f == NULL) {
    printf("Error: %s could not be opened", dictionary_file);
    exit(EXIT_FAILURE);
  }
  int lines = 0;
  for (char c = getc(f); c != EOF; c = getc(f)) {
    if (c == '\n') { lines++; }
  }
  fclose(f);
  return lines;
}

START_TEST(test_load_dictionary) {
  const char *dictionary_file = "wordlist.txt";
  int expected = count_lines(dictionary_file);
  node *hashtable[HASH_SIZE];
  load_dictionary(dictionary_file, hashtable);

  int actual = 0;
  for (int i = 0; i < HASH_SIZE; i++) {
    node *curr = hashtable[i];
    while (curr) {
      curr = curr->next;
      actual++;
    }
  }

  ck_assert_int_eq(expected, actual);
}
END_TEST

Suite *load_dictionary_suite(void) {
  Suite *s = suite_create("load_dictionary");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_load_dictionary);

  suite_add_tcase(s, tc_core);

  return s;
}
