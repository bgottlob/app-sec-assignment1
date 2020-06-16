#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include "../dictionary.h"
#include "../free_structs.h"
#include "test_suites.h"

START_TEST(test_check_word_simple) {
  const char *dictionary_file = "wordlist.txt";
  node *hashtable[HASH_SIZE];
  load_dictionary(dictionary_file, hashtable);

  ck_assert(check_word("word", hashtable));
  ck_assert(!check_word("notaword", hashtable));

  free_hashtable(hashtable);
}
END_TEST

START_TEST(test_check_word_numbers) {
  const char *dictionary_file = "wordlist.txt";
  node *hashtable[HASH_SIZE];
  load_dictionary(dictionary_file, hashtable);

  // Strings containing only digits are spelled correctly
  ck_assert(check_word("12345", hashtable));
  ck_assert(check_word("1", hashtable));
  ck_assert(check_word("95", hashtable));

  // Strings containing digits and letters are not spelled correctly
  ck_assert(!check_word("1a2", hashtable));
  ck_assert(!check_word("Hello1", hashtable));

  free_hashtable(hashtable);
}
END_TEST

START_TEST(test_check_word_special_chars) {
  const char *dictionary_file = "wordlist.txt";
  node *hashtable[HASH_SIZE];
  load_dictionary(dictionary_file, hashtable);

  // Strings containing only special characters are spelled correctly
  ck_assert(check_word("!!!", hashtable));
  ck_assert(check_word("...", hashtable));
  ck_assert(check_word(",-/", hashtable));

  // Strings containing special characters and letters are not spelled correctly
  ck_assert(!check_word("hello-There", hashtable));
  ck_assert(!check_word("And/or", hashtable));

  free_hashtable(hashtable);
}
END_TEST

Suite *check_word_suite(void) {
  Suite *s = suite_create("check_word");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_check_word_simple);
  tcase_add_test(tc_core, test_check_word_numbers);
  tcase_add_test(tc_core, test_check_word_special_chars);

  suite_add_tcase(s, tc_core);

  return s;
}
