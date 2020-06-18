---
documentclass: extarticle
author: Brandon Gottlob
fontsize: 13pt
linestretch: 2 
geometry: margin=0.75in
indent: true
header-includes: |
    \usepackage{amsmath}
    \usepackage{amssymb}
    \usepackage{ifsym}
    \usepackage{fancyhdr}
    \pagestyle{fancy}
    \fancyhead[LO,LE]{Brandon Gottlob}
    \fancyhead[RO,RE]{Assignment 1 Report}
    \fancyfoot[LO,LE]{\today}
pagestyle: empty
lang: en-US
---

# Assignment 1

## Repository Link

[https://github.com/bgottlob/app-sec-assignment1](https://github.com/bgottlob/app-sec-assignment1)

\noindent Tag for initial implementation: [https://github.com/bgottlob/app-sec-assignment1/releases/tag/v0.1](https://github.com/bgottlob/app-sec-assignment1/releases/tag/v0.1)

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
All of the following Valgrind output was run against the source code for the initial implementation tag: [https://github.com/bgottlob/app-sec-assignment1/releases/tag/v0.1](https://github.com/bgottlob/app-sec-assignment1/releases/tag/v0.1)

\noindent Valgrind output running the main function in `spell_check.c`:

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
==81080==    by 0x109642: check_words (in /home/bgottlob/Dropbox/School/summer-2020
/application-security/app-sec-assignment1/spell_check)
==81080==    by 0x109874: main (in /home/bgottlob/Dropbox/School/summer-2020
/application-security/app-sec-assignment1/spell_check)
==81080== 
==81080== 6,921,152 (105,560 direct, 6,815,592 indirect) bytes in 1,885 blocks are
definitely lost in loss record 3 of 3
==81080==    at 0x483977F: malloc (vg_replace_malloc.c:307)
==81080==    by 0x1092BF: load_dictionary (in /home/bgottlob/Dropbox/School/summer-2020
/application-security/app-sec-assignment1/spell_check)
==81080==    by 0x109854: main (in /home/bgottlob/Dropbox/School/summer-2020
/application-security/app-sec-assignment1/spell_check)
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

\noindent Valgrind output running the unit tests:

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
==81525== 6,921,152 (105,560 direct, 6,815,592 indirect) bytes in 1,885 blocks are 
definitely lost in loss record 35 of 35
==81525==    at 0x483977F: malloc (vg_replace_malloc.c:307)
==81525==    by 0x10935F: load_dictionary (in /home/bgottlob/Dropbox/School/summer-2020
/application-security/app-sec-assignment1/test)
==81525==    by 0x1098DD: test_load_dictionary_fn (in /home/bgottlob/Dropbox/School/summer-2020
/application-security/app-sec-assignment1/test)
==81525==    by 0x488BDED: srunner_run_tagged (in /usr/lib/libcheck.so.0.0.0)
==81525==    by 0x109EA7: main (in /home/bgottlob/Dropbox/School/summer-2020
/application-security/app-sec-assignment1/test)
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
==81528== 6,921,152 (105,560 direct, 6,815,592 indirect) bytes in 1,885 blocks are 
definitely lost in loss record 38 of 38
==81528==    at 0x483977F: malloc (vg_replace_malloc.c:307)
==81528==    by 0x10935F: load_dictionary (in /home/bgottlob/Dropbox/School/summer-2020
/application-security/app-sec-assignment1/test)
==81528==    by 0x109A29: test_check_word_fn (in /home/bgottlob/Dropbox/School/summer-2020
/application-security/app-sec-assignment1/test)
==81528==    by 0x488BDED: srunner_run_tagged (in /usr/lib/libcheck.so.0.0.0)
==81528==    by 0x109EA7: main (in /home/bgottlob/Dropbox/School/summer-2020
/application-security/app-sec-assignment1/test)
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
==81530==    by 0x1096E2: check_words (in /home/bgottlob/Dropbox/School/summer-2020
/application-security/app-sec-assignment1/test)
==81530==    by 0x109B6E: test_check_words_fn (in /home/bgottlob/Dropbox/School/summer-2020
/application-security/app-sec-assignment1/test)
==81530==    by 0x488BDED: srunner_run_tagged (in /usr/lib/libcheck.so.0.0.0)
==81530==    by 0x109EA7: main (in /home/bgottlob/Dropbox/School/summer-2020
/application-security/app-sec-assignment1/test)
==81530==
==81530== 6,921,152 (105,560 direct, 6,815,592 indirect) bytes in 1,885 blocks are
definitely lost in loss record 40 of 40
==81530==    at 0x483977F: malloc (vg_replace_malloc.c:307)
==81530==    by 0x10935F: load_dictionary (in /home/bgottlob/Dropbox/School/summer-2020
/application-security/app-sec-assignment1/test)
==81530==    by 0x109B34: test_check_words_fn (in /home/bgottlob/Dropbox/School/summer-2020
/application-security/app-sec-assignment1/test)
==81530==    by 0x488BDED: srunner_run_tagged (in /usr/lib/libcheck.so.0.0.0)
==81530==    by 0x109EA7: main (in /home/bgottlob/Dropbox/School/summer-2020
/application-security/app-sec-assignment1/test)
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

### Fixing Memory Leaks

The memory leaks revealed by Valgrind showed memory allocated by load_dictionary and check_words and used by the callers of those functions was not freed.
Since the caller uses that memory, it is the caller's responsibility to free the memory.
This commit adds functions for freeing memory allocated for hashtables and misspelled word lists.
These functions were then called in the spell_check.c main method and the executable check test functions.
No memory leaks occurred within `spell.c` library code, only in `spell_check.c` code, where I have implemented a main function.

\noindent After fixing the memory leaks, this is the Valgrind output running the main function in `spell_check.c` after commit [https://github.com/bgottlob/app-sec-assignment1/commit/01a5725188d6d10fb155968f9bd7641eeb6f8194](https://github.com/bgottlob/app-sec-assignment1/commit/01a5725188d6d10fb155968f9bd7641eeb6f8194):

```
$ valgrind -s --leak-check=full ./spell_check testcorpus.txt wordlist.txt
==8482== Memcheck, a memory error detector
==8482== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==8482== Using Valgrind-3.16.0.GIT and LibVEX; rerun with -h for copyright info
==8482== Command: ./spell_check testcorpus.txt wordlist.txt
==8482==
Checking `wordlist.txt` against corpus `testcorpus.txt`
5 words misspelled in `testcorpus.txt`
==8482==
==8482== HEAP SUMMARY:
==8482==     in use at exit: 0 bytes in 0 blocks
==8482==   total heap usage: 123,602 allocs, 123,602 frees, 6,931,542 bytes allocated
==8482==
==8482== All heap blocks were freed -- no leaks are possible
==8482==
==8482== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

\noindent This is the output of Valgrind when running unit tests after commit [https://github.com/bgottlob/app-sec-assignment1/commit/01a5725188d6d10fb155968f9bd7641eeb6f8194](https://github.com/bgottlob/app-sec-assignment1/commit/01a5725188d6d10fb155968f9bd7641eeb6f8194):

```
$ valgrind -s --leak-check=full ./test
==8784== Memcheck, a memory error detector
==8784== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==8784== Using Valgrind-3.16.0.GIT and LibVEX; rerun with -h for copyright info
==8784== Command: ./test
==8784==
Running suite(s): load_dictionary
==8785==
==8785== HEAP SUMMARY:
==8785==     in use at exit: 1,764 bytes in 65 blocks
==8785==   total heap usage: 123,675 allocs, 123,610 frees, 6,937,474 bytes allocated
==8785==
==8785== LEAK SUMMARY:
==8785==    definitely lost: 0 bytes in 0 blocks
==8785==    indirectly lost: 0 bytes in 0 blocks
==8785==      possibly lost: 0 bytes in 0 blocks
==8785==    still reachable: 1,764 bytes in 65 blocks
==8785==         suppressed: 0 bytes in 0 blocks
==8785== Reachable blocks (those to which a pointer was found) are not shown.
==8785== To see them, rerun with: --leak-check=full --show-leak-kinds=all
==8785==
==8785== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
 check_word
==8790==
==8790== HEAP SUMMARY:
==8790==     in use at exit: 1,856 bytes in 68 blocks
==8790==   total heap usage: 123,688 allocs, 123,620 frees, 6,946,056 bytes allocated
==8790==
==8790== LEAK SUMMARY:
==8790==    definitely lost: 0 bytes in 0 blocks
==8790==    indirectly lost: 0 bytes in 0 blocks
==8790==      possibly lost: 0 bytes in 0 blocks
==8790==    still reachable: 1,856 bytes in 68 blocks
==8790==         suppressed: 0 bytes in 0 blocks
==8790== Reachable blocks (those to which a pointer was found) are not shown.
==8790== To see them, rerun with: --leak-check=full --show-leak-kinds=all
==8790==
==8790== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
 check_words
==8791==
==8791== HEAP SUMMARY:
==8791==     in use at exit: 2,423 bytes in 72 blocks
==8791==   total heap usage: 123,719 allocs, 123,647 frees, 6,964,259 bytes allocated
==8791==
==8791== LEAK SUMMARY:
==8791==    definitely lost: 0 bytes in 0 blocks
==8791==    indirectly lost: 0 bytes in 0 blocks
==8791==      possibly lost: 0 bytes in 0 blocks
==8791==    still reachable: 2,423 bytes in 72 blocks
==8791==         suppressed: 0 bytes in 0 blocks
==8791== Reachable blocks (those to which a pointer was found) are not shown.
==8791== To see them, rerun with: --leak-check=full --show-leak-kinds=all
==8791==
==8791== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
100%: Checks: 3, Failures: 0, Errors: 0
==8784==
==8784== HEAP SUMMARY:
==8784==     in use at exit: 0 bytes in 0 blocks
==8784==   total heap usage: 123 allocs, 123 frees, 42,655 bytes allocated
==8784==
==8784== All heap blocks were freed -- no leaks are possible
==8784==
==8784== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

According to [section 4.11 of the Check library documentation](https://libcheck.github.io/check/doc/check_html/check_4.html#Finding-Memory-Leaks), the check test executor processes do not have any way of freeing all internal memory when the tests finish.
Therefore, Valgrind will always report still reachable bytes when running Check tests.
When I ran Valgrind with additional trace flags, I saw that all memory that was still reachable was allocated by `libcheck`.
This, along with the Valgrind not reporting any memory issues in the `spell_check` executable gives me confidence that my code does not have any memory leaks.

## Additional Tests
Additional tests and bug fixes were added in this commit:

[https://github.com/bgottlob/app-sec-assignment1/commit/c7beb33deb00fb62a86cb1c89c4c46d5d3249cdc](https://github.com/bgottlob/app-sec-assignment1/commit/c7beb33deb00fb62a86cb1c89c4c46d5d3249cdc)

The additional tests cover scenarios where tokens are strings whose only characters are digits or special characters, and to verify that words split by delimiters are considered to be spelled correctly. These tests found two bugs:

1. Strings with no alphabetic characters were considered misspelled
2. Hyphenated strings were evaluated as one misspelled word rather than as multiple words

## Fuzzing

I fuzzed the code by running four instances of AFL in parallel for nearly four hours and through numerous cycles.
For this first round of fuzzing, I utilized the `tests/afl-testcase/testcase.txt` file as the base testcase.
The fuzzing was performed with the corpus file as the mutating input, with the dictionary file kept the same on each run with `wordlist.txt`.
The full results are contained in the `afl-findings` directory.
Fuzzing exposed two bugs: one causing a stack smashing crash and the other causing a segmentation fault.

### First Bug - Stack Smashing

Multiple parallel instances of AFL found testcases that caused crashes.
The first I checked was in the `crashes` directory of the `fuzzer02` AFL instance.

To debug, I utilized `afl-tmin` tool, which shrinks testcases into their smallest and simplest possible forms to reproduce the same issue.
The testcase from shrinking looked like the following:
```
00000000000000000000000000000000000000000000000000000000r
```

Running this testcase through the `spell_check` binary produced by my main function in `spell_check.c` showed the same stack smashing error:
```
$ ./spell_check tests/samples/shrunk_crash_testcase.txt wordlist.txt
Checking `wordlist.txt` against corpus `tests/samples/shrunk_crash_testcase.txt`
*** stack smashing detected ***: terminated
Aborted (core dumped)
```

After a few online searches, I discovered that `gcc` adds canaries for protection against buffer overflow attacks.
A stack smashing crash occurs when a canary is overwritten and a stack check is performed.

To help figure out where in the source code the stack smashing crash is triggered, I ran my binary through Valgrind and got the following output:

```
minimizess terminating with default action of signal 6 (SIGABRT): dumping core
==318928==    at 0x48C3355: raise (in /usr/lib/libc-2.31.so)
==318928==    by 0x48AC852: abort (in /usr/lib/libc-2.31.so)
==318928==    by 0x4906877: __libc_message (in /usr/lib/libc-2.31.so)
==318928==    by 0x4996889: __fortify_fail (in /usr/lib/libc-2.31.so)
==318928==    by 0x4996853: __stack_chk_fail (in /usr/lib/libc-2.31.so)
==318928==    by 0x109632: check_word (spell.c:92)
==318928==    by 0x109766: check_words (spell.c:109)
==318928==    by 0x109A3A: main (spell_check.c:25)
```

Line 92 of `spell.c` led me to the closing bracket of the `check_word` function.
Adding in some print statements as well showed me that the crash was occurring after the function finished executing but before the caller in the `check_words` function regained control and received the returned value of `check_word`.
This led me to believe the error occurred when the stack frame for the `check_word` function that had just finished executing was being deallocated, meaning some memory allocated on `check_word`'s stack was likely the cause.

The only new memory allocated in `check_word` is the `lower` array, which is used to hold the lower cased word:

``` c
char lower[LENGTH + 1];
```

Understanding that the input from the shrunk testcase is a token that is 57 characters long and `LENGTH + 1` evaluates to 46, it became clear that not enough memory was being allocated to `lower`.
The `word_tolower` function was looping based on the length of the word, not the length of this lower array, so a word string (`str`) that is larger than what `lower` can hold will cause this loop to exceed `lower`'s memory boundary

``` c
for (i = 0; str[i] != 0; i++) {
  char lower = tolower(str[i]);
  if (lower >= 'a' && lower <= 'z') { return true; }
}
```


The fix checks if the word exceeds the maximum word length in the dictionary.
If so, the word cannot possibly be spelled correctly, so false is returned.

``` c
if (strlen(word) > LENGTH) { return false; }
```

For added safety `strncpy` is now used to copy the word into `lower`, then only the `lower` string is passed to the `word_tolower`.
This way, there will be no mismatches in array length since it is limited at `LENGTH + 1`.

``` c
strncpy(lower, word, LENGTH + 1);
```

After fixing the bug and verifying manually that the fixes work, I reran the existing unit tests, then added a unit test to `tests/test_check_words.c` for the shrunk testcase that caused the crash before the fix.

### Second Bug - Invalid Hash Value

After fixing the first bug, I tested the crashing input that was found by fuzzing by `fuzzer03` on a different processor core.
I started again by shrinking the testcase and running it through my `spell_check` binary.
This error from the program was a segmentation fault, so I then ran it through Valgrind.
Valgrind presented a lot of output, but I found this to be the most helpful:

```
==336760== Invalid read of size 1
==336760==    at 0x49E49A0: __strcmp_avx2 (in /usr/lib/libc-2.31.so)
==336760==    by 0x1094CD: check_word_exact (in /home/bgottlob/Dropbox/School/summer-2020
/application-security/app-sec-assignment1/spell_check)
==336760==    by 0x109617: check_word (in /home/bgottlob/Dropbox/School/summer-2020
/application-security/app-sec-assignment1/spell_check)
==336760==    by 0x109771: check_words (in /home/bgottlob/Dropbox/School/summer-2020
/application-security/app-sec-assignment1/spell_check)
==336760==    by 0x109A57: main (in /home/bgottlob/Dropbox/School/summer-2020
/application-security/app-sec-assignment1/spell_check)
==336760==  Address 0x6f746e616b2f6c61 is not stack'd, malloc'd or (recently) free'd
```

This pointed me to the `check_word_exact` function.
Having just fixed an array indexing bug, I looked at the only array access in `check_word_exact`:

``` c
hashtable[hash_function(hash)]
```

I was clearly assuming `hash_function` would always return me a valid value between 0 and `HASH_SIZE - 1`.
I inspected the shrunk testcase file, which contained some strange-looking non-ASCII characters.
When printing their integer values out in C, I saw that they had the value `-70` and caused the `hash_function` to return a negative number.

To fix this, I added this check to `check_word_exact`:
```
if (hash >= 0 && hash < HASH_SIZE)
```

I also added this after the call to `hash_function` call in `load_dictionary` so the same problem cannot occur there.
Like for the first bug, I added this shrunk testcase as a unit test, except this time for both `check_words` and `load_dictionary`.

### After the Bug Fixes

This commit contains fixes to both of the bugs found from fuzzing: [https://github.com/bgottlob/app-sec-assignment1/commit/d2b42a817ce8c503676db2b813dc849e182fbde0](https://github.com/bgottlob/app-sec-assignment1/commit/d2b42a817ce8c503676db2b813dc849e182fbde0).
After making this commit, I ran four parallel AFL instances on the changes.
After running for nearly two hours with many completed cycles, there were no crashes or hangs.
The results of this second round of fuzzing are contained in the `afl-findings-after-fixes` directory.

### Fuzzing a Dictionary File

After making the bug fixes, I fuzzed by keeping the corpus file constant as `tests/afl-testcases/testcase.txt` and utilized `tests/afl-testcase-dictionary/wordlist.txt` as a seed for fuzzing mutations.
This dictionary file is much smaller than `wordlist.txt` to improve performance of the fuzzing.
Ultimately, this proved to be very useful in fuzzing comprehensively very quickly, as AFL performed 1000 full cycles on a single processor core in less than 10 minutes.
The full results of this round of fuzzing are located in `afl-findings-dictionary`.
There were two inputs that AFL detected as program hangs, but when running them through my program manually, they returned in no more than two seconds.
The hangs seem to have been false positives, and can be safely ignored, and thus there were no bugs that were found as part of this round of fuzzing
This gave me extra coverage over fuzzing inputs into the `load_dictionary` function.

## Avoiding Similar Bugs in the Future

The bugs detected by unit tests were special cases that I did not consider in my initial implementation.
Since unit tests exist for those cases, they should not resurface without the CI process catching them.

Fuzzing exposed two bugs, both of which were caused by out-of-bounds array indexing.
While creating new code, I need to more carefully consider the range of values an integer value can take when I use it to index an array, and be especially careful when dynamically allocated memory is involved.
Any untrusted array bounds values need to be validated before using them to index an array.
The path to trusting a value would need to involve thoroughly unit testing the functions that generate them.

Fuzzing proved to be an excellent tool for exposing this class of bugs.
However, it is not simple to integrate fuzzing into a Travis CI pipeline as it is to do so for unit tests.
The nature of fuzzing is dynamic and requires a judgement call to know when to stop.
Dedicated cloud hardware for running fuzzer jobs could be a good solution for creating fuzzing jobs in CI.
Even with that available, I have seen that fuzzing can have false positives when it comes to hanging as well.
Given these problems, fuzzing can run locally when a significant code change (or set of changes) is made, since its potentially high cost would add a lot of overhead to CI if done with every single code change.

As a lower overhead memory safety check, I have a separate Travis CI stage that runs Valgrind on the `./spell_check` and `./test` executables.
If memory leaks are found, the CI stage will fail.
An exception is made for the `./test` executable, due to the memory deallocation limitation with the Check library discussed previously.
Clearly, Valgrind will not detect all of the memory access issues that fuzzing can find, but it is easy to run in CI against every code change.
