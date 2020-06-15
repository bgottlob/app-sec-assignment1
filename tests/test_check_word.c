#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include "../dictionary.h"
#include "../free_structs.h"
#include "test_suites.h"

START_TEST(test_check_word) {
  const char *dictionary_file = "wordlist.txt";
  node *hashtable[HASH_SIZE];
  load_dictionary(dictionary_file, hashtable);

  ck_assert(check_word("word", hashtable));
  ck_assert(!check_word("notaword", hashtable));

  free_hashtable(hashtable);
}
END_TEST

Suite *check_word_suite(void) {
  Suite *s = suite_create("check_word");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_check_word);

  suite_add_tcase(s, tc_core);

  return s;
}
