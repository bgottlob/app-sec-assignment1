#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include "../dictionary.h"

int count_lines(const char* dictionary_file) {
  FILE *f = fopen(dictionary_file, "r");
  if (f == NULL) {
    printf("Error: %s could not be opened");
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

Suite *spell_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Spell");

  /* Core test case */
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_load_dictionary);
  suite_add_tcase(s, tc_core);

  return s;
}

int main(void) {
  int number_failed;

  Suite *s;
  SRunner *sr;

  s = spell_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
