# Assignment 1

## Repository Link

https://github.com/bgottlob/app-sec-assignment1

Tag for initial implementation: https://github.com/bgottlob/app-sec-assignment1/releases/tag/v0.1

## Description of Program

`load_dictionary` starts by opening the dictionary file and setting all entries in the hashtable array to `NULL` pointers.
Each line of the dictionary file is read into a buffer using `fgets`, then the trailing newline character is removed.
Memory is then allocated for a new node in the hashtable, where the word is set to the content of the line from the file and the next pointer is set to `NULL`.
The hash is computed for the contents of the buffer using `hash_function`.
The corresponding entry in the hashtable is read.
If the entry is a `NULL` pointer, the new node becomes that hashtable entry.
If the entry is already a node, that node's, and subsequent nodes', next pointers are traversed to the end, and the newly allocated node is placed at the end
This process repeats for every word in the file.

`check_word` computes the `hash_function` of the given word to find the entry in the hashtable it must be stored in, if at all.
Each node in that hashtable entry is traversed, comparing the input word with that of the node until a match is found.
If a match is found, the function returns `true`.
Otherwise, the string is set to all lowercase letters and the hashtable is searched once again.
To make the code less repetitive, a `check_word_exact` function is used to implement the hashtable search.

`check_words` sets each entry in the misspelled word array to `NULL` and a counter of misspelled words is set to 0.
Each line of the opened input file from the `FILE` pointer is read into a bounded buffer.
Each line is then tokenized according to a set of delimiter punctuation and whitespace characters.
`check_word` is called on each token.
If `check_word` is true, the next token is checked.
If `check_word` is false, the counter increments and memory is allocated to store the word in the misspelled word list.
After all lines from the file are read, the counter is returned.

## Expected Bugs

I expect off-by-one errors in allocating memory for buffers to hold words.
The `dictionary.h` file defines a `LENGTH` constant to be the longest word possible.
All buffers to hold individual words to check are set to a size of `LENGTH + 1`, to leave one byte at the end for the NULL terminator byte.
I expect there may be some places where that extra byte is forgotten and a string is terminated one character early.

I expect there to be some bugs due to the bounded size of buffers used to hold
each line of the input files.
The size of the buffer to hold each line is arbitrary, and files with long enough lines will be cut off.

There will also likely be bugs with hyphenated words or words with numeric
characters since they are less common cases.

Any memory that is allocated dynamically using `malloc` is allocated for the caller to use, so it is up to the caller to free.
I do not suspect there will be any major memory allocation bugs that have not been caught while initally writing this code, but there may be some minor ones since manual memory allocation is error prone in general.

## Valgrind Output
All of the following Valgrind output was run against the source code for the initial implementation tag: https://github.com/bgottlob/app-sec-assignment1/releases/tag/v0.1

Valgrind output running the main function in `spell_check.c`:

```
$ valgrind -s --leak-check=full ./spell_check testcorpus.txt wordlist.txt
==81080== Memcheck, a memory error detector
==81080== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==81080== Using Valgrind-3.16.0.GIT and LibVEX; rerun with -h for copyright info
==81080== Command: ./spell_check testcorpus.txt wordlist.txt
==81080== 
Checking `wordlist.txt` against corpus `testcorpus.txt`
5 words misspelled in `testcorpus.txt`
==81080== 
==81080== HEAP SUMMARY:
==81080==     in use at exit: 6,921,382 bytes in 123,597 blocks
==81080==   total heap usage: 123,602 allocs, 5 frees, 6,931,542 bytes allocated
==81080== 
==81080== 230 bytes in 5 blocks are definitely lost in loss record 1 of 3
==81080==    at 0x483977F: malloc (vg_replace_malloc.c:307)
==81080==    by 0x109642: check_words (in /home/bgottlob/Dropbox/School/summer-2020/application-security/app-sec-assignment1/spell_check)
==81080==    by 0x109874: main (in /home/bgottlob/Dropbox/School/summer-2020/application-security/app-sec-assignment1/spell_check)
==81080== 
==81080== 6,921,152 (105,560 direct, 6,815,592 indirect) bytes in 1,885 blocks are definitely lost in loss record 3 of 3
==81080==    at 0x483977F: malloc (vg_replace_malloc.c:307)
==81080==    by 0x1092BF: load_dictionary (in /home/bgottlob/Dropbox/School/summer-2020/application-security/app-sec-assignment1/spell_check)
==81080==    by 0x109854: main (in /home/bgottlob/Dropbox/School/summer-2020/application-security/app-sec-assignment1/spell_check)
==81080== 
==81080== LEAK SUMMARY:
==81080==    definitely lost: 105,790 bytes in 1,890 blocks
==81080==    indirectly lost: 6,815,592 bytes in 121,707 blocks
==81080==      possibly lost: 0 bytes in 0 blocks
==81080==    still reachable: 0 bytes in 0 blocks
==81080==         suppressed: 0 bytes in 0 blocks
==81080== 
==81080== ERROR SUMMARY: 2 errors from 2 contexts (suppressed: 0 from 0)
```

Valgrind output running the unit tests:

```
$ valgrind -s --leak-check=full ./test
==81524== Memcheck, a memory error detector
==81524== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==81524== Using Valgrind-3.16.0.GIT and LibVEX; rerun with -h for copyright info
==81524== Command: ./test
==81524==
Running suite(s): Spell
==81525==
==81525== HEAP SUMMARY:
==81525==     in use at exit: 6,922,292 bytes in 123,625 blocks
==81525==   total heap usage: 123,643 allocs, 18 frees, 6,936,854 bytes allocated
==81525==
==81525== 6,921,152 (105,560 direct, 6,815,592 indirect) bytes in 1,885 blocks are definitely lost in loss record 35 of 35
==81525==    at 0x483977F: malloc (vg_replace_malloc.c:307)
==81525==    by 0x10935F: load_dictionary (in /home/bgottlob/Dropbox/School/summer-2020/application-security/app-sec-assignment1/test)
==81525==    by 0x1098DD: test_load_dictionary_fn (in /home/bgottlob/Dropbox/School/summer-2020/application-security/app-sec-assignment1/test)
==81525==    by 0x488BDED: srunner_run_tagged (in /usr/lib/libcheck.so.0.0.0)
==81525==    by 0x109EA7: main (in /home/bgottlob/Dropbox/School/summer-2020/application-security/app-sec-assignment1/test)
==81525==
==81525== LEAK SUMMARY:
==81525==    definitely lost: 105,560 bytes in 1,885 blocks
==81525==    indirectly lost: 6,815,592 bytes in 121,707 blocks
==81525==      possibly lost: 0 bytes in 0 blocks
==81525==    still reachable: 1,140 bytes in 33 blocks
==81525==         suppressed: 0 bytes in 0 blocks
==81525== Reachable blocks (those to which a pointer was found) are not shown.
==81525== To see them, rerun with: --leak-check=full --show-leak-kinds=all
==81525==
==81525== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
==81528==
==81528== HEAP SUMMARY:
==81528==     in use at exit: 6,922,385 bytes in 123,628 blocks
==81528==   total heap usage: 123,656 allocs, 28 frees, 6,945,472 bytes allocated
==81528==
==81528== 6,921,152 (105,560 direct, 6,815,592 indirect) bytes in 1,885 blocks are definitely lost in loss record 38 of 38
==81528==    at 0x483977F: malloc (vg_replace_malloc.c:307)
==81528==    by 0x10935F: load_dictionary (in /home/bgottlob/Dropbox/School/summer-2020/application-security/app-sec-assignment1/test)
==81528==    by 0x109A29: test_check_word_fn (in /home/bgottlob/Dropbox/School/summer-2020/application-security/app-sec-assignment1/test)
==81528==    by 0x488BDED: srunner_run_tagged (in /usr/lib/libcheck.so.0.0.0)
==81528==    by 0x109EA7: main (in /home/bgottlob/Dropbox/School/summer-2020/application-security/app-sec-assignment1/test)
==81528==
==81528== LEAK SUMMARY:
==81528==    definitely lost: 105,560 bytes in 1,885 blocks
==81528==    indirectly lost: 6,815,592 bytes in 121,707 blocks
==81528==      possibly lost: 0 bytes in 0 blocks
==81528==    still reachable: 1,233 bytes in 36 blocks
==81528==         suppressed: 0 bytes in 0 blocks
==81528== Reachable blocks (those to which a pointer was found) are not shown.
==81528== To see them, rerun with: --leak-check=full --show-leak-kinds=all
==81528==
==81528== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
==81530==
==81530== HEAP SUMMARY:
==81530==     in use at exit: 6,923,188 bytes in 123,637 blocks
==81530==   total heap usage: 123,687 allocs, 50 frees, 6,963,745 bytes allocated
==81530==
==81530== 230 bytes in 5 blocks are definitely lost in loss record 36 of 40
==81530==    at 0x483977F: malloc (vg_replace_malloc.c:307)
==81530==    by 0x1096E2: check_words (in /home/bgottlob/Dropbox/School/summer-2020/application-security/app-sec-assignment1/test)
==81530==    by 0x109B6E: test_check_words_fn (in /home/bgottlob/Dropbox/School/summer-2020/application-security/app-sec-assignment1/test)
==81530==    by 0x488BDED: srunner_run_tagged (in /usr/lib/libcheck.so.0.0.0)
==81530==    by 0x109EA7: main (in /home/bgottlob/Dropbox/School/summer-2020/application-security/app-sec-assignment1/test)
==81530==
==81530== 6,921,152 (105,560 direct, 6,815,592 indirect) bytes in 1,885 blocks are definitely lost in loss record 40 of 40
==81530==    at 0x483977F: malloc (vg_replace_malloc.c:307)
==81530==    by 0x10935F: load_dictionary (in /home/bgottlob/Dropbox/School/summer-2020/application-security/app-sec-assignment1/test)
==81530==    by 0x109B34: test_check_words_fn (in /home/bgottlob/Dropbox/School/summer-2020/application-security/app-sec-assignment1/test)
==81530==    by 0x488BDED: srunner_run_tagged (in /usr/lib/libcheck.so.0.0.0)
==81530==    by 0x109EA7: main (in /home/bgottlob/Dropbox/School/summer-2020/application-security/app-sec-assignment1/test)
==81530==
==81530== LEAK SUMMARY:
==81530==    definitely lost: 105,790 bytes in 1,890 blocks
==81530==    indirectly lost: 6,815,592 bytes in 121,707 blocks
==81530==      possibly lost: 0 bytes in 0 blocks
==81530==    still reachable: 1,806 bytes in 40 blocks
==81530==         suppressed: 0 bytes in 0 blocks
==81530== Reachable blocks (those to which a pointer was found) are not shown.
==81530== To see them, rerun with: --leak-check=full --show-leak-kinds=all
==81530==
==81530== ERROR SUMMARY: 2 errors from 2 contexts (suppressed: 0 from 0)
100%: Checks: 3, Failures: 0, Errors: 0
==81524==
==81524== HEAP SUMMARY:
==81524==     in use at exit: 0 bytes in 0 blocks
==81524==   total heap usage: 91 allocs, 91 frees, 42,141 bytes allocated
==81524==
==81524== All heap blocks were freed -- no leaks are possible
==81524==
==81524== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```
