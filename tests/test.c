#include <check.h>
#include <stdlib.h>
#include "test_suites.h"

// main function for check tests
int main(void) {
  int number_failed;

  SRunner *sr = srunner_create(load_dictionary_suite());
  srunner_add_suite(sr, check_word_suite());
  srunner_add_suite(sr, check_words_suite());

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
