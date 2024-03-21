# Spell Checker Program README

## Overview

This spell checker program is a console-based application designed to load a dictionary of words
from a file into a hash table, check input text for spelling errors, suggest possible corrections
based on edit distance, allow users to add new words to the dictionary, handle hash table collisions
using separate chaining, and implement a caching mechanism for correction suggestions. It offers the
capability to spell check a file in real-time and automatically replaces the words in the file with
the corrected suggestions, ensuring a seamless and efficient correction process.

## Approach

The program is implemented in C++ and uses data structures implemented in the standard templating
library for storing the dictionary. The hash table employs separate chaining to resolve collisions,
ensuring efficient handling of insertions, deletions, and searches. A significant enhancement
includes a caching system for frequently suggested corrections, reducing the computation time for
repeated misspellings.

### Key Components:

- **Levenshtein Distance Algorithm**: Utilized to calculate the edit distance between two words,
  enabling the suggestion of corrections for misspelled words.
- **Hash Table with Separate Chaining**: Employs a hash table to store the dictionary, offering
  efficient lookup and insertion performance.
- **Caching System**: A caching mechanism has been introduced to store recent suggestions for
  misspelled words, significantly speeding up the correction process for words that have already
  been encountered.
- **Real-time File Correction**: Enables users to spell check files directly, with the ability to
  replace misspelled words in the file with suggested corrections in real-time, preserving the
  context and formatting of the original text.
- **User Interface**: Features a simple, menu-driven command-line interface that guides the user
  through the process of loading dictionaries, spell checking text or files, adding new words, and
  applying suggestions.

## Optimization Techniques

- **Separate Chaining for Collision Resolution**: Ensures consistent lookup times and efficient
  performance of dictionary operations, even as the dictionary size grows.
- **Dynamic Hash Table Resizing**: Automatically adjusts the size of the hash table based on the
  current load factor, optimizing the balance between memory usage and access time.
- **Caching for Correction Suggestions**: Significantly reduces the time required to suggest
  corrections for previously encountered misspellings by avoiding redundant computations.

## Performance Measurements

Performance measurements were conducted to evaluate the efficiency of the spell checker program in
terms of time complexity and memory usage, particularly with the enhancements for caching
suggestions and real-time file correction.

- **Load Time**: Measures the time taken to load the dictionary into the hash table.
- **Spell Check Time**: Assesses the time required to identify misspelled words in the given text or
  file.
- **Suggestion Generation Time**: Evaluates the efficiency of generating suggestions for misspelled
  words, notably improved with the caching mechanism.

## User Manual

### Running the Program

1. Compile the program using a C++ compiler, ensuring all required files are included.
2. Launch the program from a command-line interface.

### Menu Options

- **[L] Load Dictionary**: Prompts for a dictionary file to load into the hash table. This is
  essential for the spell checker to function, as it provides the reference words that the spell
  checker uses to identify and correct misspellings.

- **[C] Check Spelling**: Initiates spell checking for entered text, offering real-time corrections.
  This option is designed for quick checks of small amounts of text, allowing for immediate
  correction suggestions that the user can apply to their text.

- **[F] Check Spelling and Correct File**: A new feature that extends the spell checker's
  capabilities to entire files. This option lets the user specify a file whose content will be
  checked for spelling errors. Corrections are suggested for any misspelled words found, and with
  user approval, these corrections can be applied directly to the file, streamlining the editing
  process.

- **[A] Add Word to Dictionary**: Allows adding a new word to the dictionary. This feature is
  particularly useful for including words that are not part of the standard dictionary, ensuring
  they are not flagged as errors in future corrections.

- **[P] Purge Cache**: Clears the cache of suggested corrections. Over time, the cache might grow or
  contain outdated suggestions. This option provides a way to refresh the cache, potentially
  improving the performance and relevance of correction suggestions.

- **[Q] Quit**: Exits the program. This option safely closes the spell checker application.

### Adding a New Dictionary

Ensure the dictionary file is in plain text format, with one word per line. Specify the file path
when prompted by the **[L] Load Dictionary** option.

## Conclusion

With the introduction of a caching system for correction suggestions and the ability to correct
files in real-time, the spell checker program significantly enhances its efficiency and
user-friendliness. These features, combined with the robust foundation of using appropriate data
structures and the Levenshtein distance algorithm, make it a powerful tool for addressing spelling
errors swiftly and accurately.
