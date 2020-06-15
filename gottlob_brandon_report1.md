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
