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

// TODO move this to its own file
START_TEST(test_check_word) {
  const char *dictionary_file = "wordlist.txt";
  node *hashtable[HASH_SIZE];
  load_dictionary(dictionary_file, hashtable);

  ck_assert(check_word("word", hashtable));
  ck_assert(!check_word("notaword", hashtable));
}
END_TEST

// TODO move this to its own file
START_TEST(test_check_words) {
  const char *dictionary_file = "wordlist.txt";
  node *hashtable[HASH_SIZE];
  load_dictionary(dictionary_file, hashtable);

  FILE *corpusfp = fopen("testcorpus.txt", "r");
  char *misspelled[MAX_MISSPELLED];
  int numMisspelled = check_words(corpusfp, hashtable, misspelled);
  
  ck_assert_int_eq(5, numMisspelled);
  ck_assert(strcmp("sogn", misspelled[0]) == 0);
  ck_assert(strcmp("skyn", misspelled[1]) == 0);
  ck_assert(strcmp("betta", misspelled[2]) == 0);
  ck_assert(strcmp("Teh", misspelled[3]) == 0);
  ck_assert(strcmp("ovre", misspelled[4]) == 0);
}
END_TEST

Suite *spell_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Spell");

  /* Core test case */
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_load_dictionary);
  tcase_add_test(tc_core, test_check_word);
  tcase_add_test(tc_core, test_check_words);
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
