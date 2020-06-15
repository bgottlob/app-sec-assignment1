#include <check.h>
#include <stdio.h>
#include "../dictionary.h"
#include "test_suites.h"

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

Suite *check_words_suite(void) {
  Suite *s = suite_create("check_words");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_check_words);

  suite_add_tcase(s, tc_core);

  return s;
}
