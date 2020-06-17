#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include "../dictionary.h"
#include "../free_structs.h"
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

  free_hashtable(hashtable);
}
END_TEST

/* 
 * Use a testcase that was found by the AFL fuzzer that previously caused a
 * seg fault due to hash_function being evaluated to a negative number
 */
START_TEST(test_load_dictionary_segfault) {
  const char *dictionary_file = "tests/samples/shrunk_segfault_testcase.txt";
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

  // The file contains one word that causes the hash_function to evaluate to
  // an invalid negative number. The word is ignored in that case
  ck_assert_int_eq(0, actual);

  free_hashtable(hashtable);
}
END_TEST

Suite *load_dictionary_suite(void) {
  Suite *s = suite_create("load_dictionary");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_load_dictionary);
  tcase_add_test(tc_core, test_load_dictionary_segfault);

  suite_add_tcase(s, tc_core);

  return s;
}
