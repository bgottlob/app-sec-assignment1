#include <check.h>
#include <stdio.h>
#include "../dictionary.h"
#include "../free_structs.h"
#include "test_suites.h"

START_TEST(test_check_words_simple) {
  const char *dictionary_file = "wordlist.txt";
  node *hashtable[HASH_SIZE];
  load_dictionary(dictionary_file, hashtable);

  FILE *corpusfp = fopen("tests/samples/testcorpus.txt", "r");
  char *misspelled[MAX_MISSPELLED];
  int num_misspelled = check_words(corpusfp, hashtable, misspelled);
  
  ck_assert_int_eq(5, num_misspelled);
  ck_assert(strcmp("sogn", misspelled[0]) == 0);
  ck_assert(strcmp("skyn", misspelled[1]) == 0);
  ck_assert(strcmp("betta", misspelled[2]) == 0);
  ck_assert(strcmp("Teh", misspelled[3]) == 0);
  ck_assert(strcmp("ovre", misspelled[4]) == 0);

  free_hashtable(hashtable);
  free_misspelled(misspelled);
}
END_TEST

START_TEST(test_check_words_delimiters) {
  const char *dictionary_file = "wordlist.txt";
  node *hashtable[HASH_SIZE];
  load_dictionary(dictionary_file, hashtable);

  FILE *corpusfp = fopen("tests/samples/test_special_chars_corpus.txt", "r");
  char *misspelled[MAX_MISSPELLED];
  int num_misspelled = check_words(corpusfp, hashtable, misspelled);

  // Tests that various delimiters caused strings to split into multiple tokens
  ck_assert_int_eq(2, num_misspelled);
  ck_assert(strcmp("Hodor", misspelled[0]) == 0);
  ck_assert(strcmp("Hodor", misspelled[1]) == 0);

  free_hashtable(hashtable);
  free_misspelled(misspelled);
}
END_TEST

START_TEST(test_check_words_numeric) {
  const char *dictionary_file = "wordlist.txt";
  node *hashtable[HASH_SIZE];
  load_dictionary(dictionary_file, hashtable);

  FILE *corpusfp = fopen("tests/samples/test_numeric_chars_corpus.txt", "r");
  char *misspelled[MAX_MISSPELLED];
  int num_misspelled = check_words(corpusfp, hashtable, misspelled);

  // Tests that digit-only strings are words, even when delimiters split them
  ck_assert_int_eq(0, num_misspelled);

  free_hashtable(hashtable);
  free_misspelled(misspelled);
}
END_TEST

START_TEST(test_check_words_longest) {
  const char *dictionary_file = "tests/sample_dictionaries/longest_wordlist.txt";
  node *hashtable[HASH_SIZE];
  load_dictionary(dictionary_file, hashtable);

  FILE *corpusfp = fopen("tests/samples/test_longest_word.txt", "r");
  char *misspelled[MAX_MISSPELLED];
  int num_misspelled = check_words(corpusfp, hashtable, misspelled);

  // Tests that digit-only strings are words, even when delimiters split them
  ck_assert_int_eq(1, num_misspelled);
  ck_assert_str_eq(
    "pneumonoultramicroscopicsilicovolcanoconiosix",
    misspelled[0]
  );

  free_hashtable(hashtable);
  free_misspelled(misspelled);
}
END_TEST

/* 
 * Use a testcase that was found by the AFL fuzzer that previously caused a
 * stack smashing error
 */
START_TEST(test_check_words_stack_smash) {
  const char *dictionary_file = "wordlist.txt";
  node *hashtable[HASH_SIZE];
  load_dictionary(dictionary_file, hashtable);

  FILE *corpusfp = fopen("tests/samples/shrunk_crash_testcase.txt", "r");
  char *misspelled[MAX_MISSPELLED];
  int num_misspelled = check_words(corpusfp, hashtable, misspelled);

  ck_assert_int_eq(1, num_misspelled);
  ck_assert_str_eq(
    "00000000000000000000000000000000000000000000000000000000r",
    misspelled[0]
  );

  free_hashtable(hashtable);
  free_misspelled(misspelled);
}
END_TEST

/* 
 * Use a testcase that was found by the AFL fuzzer that previously caused a
 * seg fault due to hash_function being evaluated to a negative number
 */
START_TEST(test_check_words_segfault) {
  const char *dictionary_file = "wordlist.txt";
  node *hashtable[HASH_SIZE];
  load_dictionary(dictionary_file, hashtable);

  FILE *corpusfp = fopen("tests/samples/shrunk_segfault_testcase.txt", "r");
  char *misspelled[MAX_MISSPELLED];
  int num_misspelled = check_words(corpusfp, hashtable, misspelled);

  ck_assert_int_eq(1, num_misspelled);

  free_hashtable(hashtable);
  free_misspelled(misspelled);
}
END_TEST

Suite *check_words_suite(void) {
  Suite *s = suite_create("check_words");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_check_words_simple);
  tcase_add_test(tc_core, test_check_words_delimiters);
  tcase_add_test(tc_core, test_check_words_numeric);
  tcase_add_test(tc_core, test_check_words_longest);
  tcase_add_test(tc_core, test_check_words_stack_smash);
  tcase_add_test(tc_core, test_check_words_segfault);

  suite_add_tcase(s, tc_core);

  return s;
}
