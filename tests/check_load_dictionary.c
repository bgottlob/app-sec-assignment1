#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include "../dictionary.h"

START_TEST(test_load_dictionary) {
  char output[1000];
  FILE *fp = popen("/bin/wc -l wordlist.txt", "r");
  if (fp == NULL) {
    printf("Failed to run wc -l\n" );
    exit(1);
  }

  while (fgets(output, sizeof(output), fp) != NULL) {
    printf("%s", output);
  }

  pclose(fp);

  int expected = atoi(output);

  node *hashtable[HASH_SIZE];
  load_dictionary("wordlist.txt", hashtable);

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
