//
// Copyright Caiden Sanders - All Rights Reserved
//
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
//
// Written by Caiden Sanders <work.caidensanders@gmail.com>, March 18, 2024.
//

// Input/Output Includes
#include <fstream>
#include <iostream>
#include <sstream>

// Data Structure Includes
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// Algorithm Includes
#include <algorithm>
#include <limits>

// Function Prototypes
int levenshtein_distance(const std::string& word1, const std::string& word2);
std::unordered_map<std::string, bool> load_dictionary(
    const std::string& filename);
std::vector<std::string> spell_check(
    const std::string& text, const std::unordered_set<std::string>& dictionary);
std::vector<std::pair<std::string, std::string>> suggest_corrections(
    const std::vector<std::string>& misspelled,
    const std::unordered_set<std::string>& dictionary);
void print_results(
    const std::vector<std::string>& misspelled,
    const std::vector<std::pair<std::string, std::string>>& corrections);
void add_word_to_dictionary(
    const std::unordered_map<std::string, bool>& dictionary,
    const std::string& word);

// Global Cache
std::unordered_map<std::string, std::string> cache;

/**
 * Implementation of the Levenshtein distance algorithm to calculate the
 * minimum number of single-character edits (insertions, deletions, or
 * substitutions) required to change one word into another.
 *
 * @param word1 The first word.
 * @param word2 The second word.
 * @return The Levenshtein distance between the two words.
 * @see https://en.wikipedia.org/wiki/Levenshtein_distance
 */
int levenshtein_distance(const std::string& word1, const std::string& word2) {
    // Create a 2D array to store the distances between prefixes of the two
    // words.
    std::vector<std::vector<int>> distances(word1.size() + 1,
                                            std::vector<int>(word2.size() + 1));

    // Initialize the first row and column of the array with the distances from
    // the empty string to each prefix of the words.
    for (int i = 0; i <= word1.size(); i++) {
        distances[i][0] = i;
    }
    for (int j = 0; j <= word2.size(); j++) {
        distances[0][j] = j;
    }

    // Calculate the minimum distance between prefixes of the words.
    for (int i = 1; i <= word1.size(); i++) {
        for (int j = 1; j <= word2.size(); j++) {
            if (word1[i - 1] == word2[j - 1]) {
                distances[i][j] = distances[i - 1][j - 1];
            } else {
                distances[i][j] =
                    1 + std::min({distances[i - 1][j], distances[i][j - 1],
                                  distances[i - 1][j - 1]});
            }
        }
    }

    // The Levenshtein distance is the value in the bottom-right corner of the
    // array.
    return distances[word1.size()][word2.size()];
}

/**
 * Load a dictionary of words from a file into a hash table.
 *
 * @param filename The name of the file containing the dictionary.
 * @return A hash table containing the words from the dictionary.
 */
std::unordered_map<std::string, bool> load_dictionary(
    const std::string& filename) {
    std::unordered_map<std::string, bool> dictionary(100);
    std::ifstream file;

    file.open(filename);
    if (!file) {
        std::cerr << "Error: could not open " << filename << std::endl;
        return dictionary;
    }

    std::string word;
    while (file >> word) {
        dictionary[word] = true;
    }

    file.close();

    return dictionary;
}

/**
 * Add a new word to the dictionary stored in the hash table.
 *
 * @param dictionary The hash table containing the dictionary of words.
 */
void add_word_to_dictionary(std::unordered_map<std::string, bool>& dictionary) {
    std::string new_word;

    std::cout << "Enter the word to add to the dictionary: ";
    std::getline(std::cin, new_word);

    auto result = dictionary.insert({new_word, true});

    if (result.second) {
        std::cout << "Word added successfully." << std::endl;
    } else {
        std::cout << "Word already exists in the dictionary." << std::endl;
    }
}

/**
 * Take a string of text as input and check each word in the text against the
 * words in the dictionary stored in the hash table. Identify any words that
 * are not found in the dictionary and display them as "mispelled".
 *
 * @param text The string of text to check.
 * @param dictionary The hash table containing the dictionary of words.
 * @return A vector of misspelled words.
 */
std::vector<std::string> spell_check(
    const std::string& text,
    const std::unordered_set<std::string>& dictionary) {
    std::vector<std::string> misspelled;
    std::string word;
    std::string current_word;
    std::istringstream textStream(text);

    while (textStream >> word) {
        if (dictionary.find(word) == dictionary.end()) {
            misspelled.push_back(word);
        }
    }

    return misspelled;
}

/**
 * ----------------------------------------------------------------------------
 * DEPRECATED: Replaced by suggest_corrections_cached.
 * ----------------------------------------------------------------------------
 *
 * Based off of a vector of mispelled words and a dictionary stored in a
 * hash table, suggest a correction for each misspelled word using the
 * Levenshtein distance algorithm. Only include words that are likely
 * to be mispelled and have a distance that is related to the size
 * of the word.
 *
 * @param misspelled A vector of misspelled words.
 * @param dictionary The hash table containing the dictionary of words.
 * @return A vector of pairs, where each pair contains a misspelled word and
 *         its suggested correction.
 */
std::vector<std::pair<std::string, std::string>> suggest_corrections(
    const std::vector<std::string>& misspelled,
    const std::unordered_set<std::string>& dictionary) {
    std::vector<std::pair<std::string, std::string>> corrections;

    for (const auto& word : misspelled) {
        std::string best_match;
        int best_distance = std::numeric_limits<int>::max();

        for (const auto& entry : dictionary) {
            int distance = levenshtein_distance(word, entry);

            if (distance < best_distance) {
                best_distance = distance;
                best_match = entry;
            }
        }

        if (best_distance <= 2 && !best_match.empty()) {
            corrections.push_back({word, best_match});
        }
    }

    return corrections;
}

std::vector<std::pair<std::string, std::string>> suggest_corrections_cached(
    const std::vector<std::string>& misspelled,
    const std::unordered_set<std::string>& dictionary) {
    std::vector<std::pair<std::string, std::string>> corrections;

    for (const auto& word : misspelled) {
        // Check if the word is already in the cache.
        auto cached = cache.find(word);
        if (cached != cache.end()) {
            corrections.push_back({word, cached->second});
            continue;
        }

        // If the word is not in the cache, find the best match in the
        // dictionary and add it to the cache.
        for (const auto& entry : dictionary) {
            int distance = levenshtein_distance(word, entry);

            if (distance <= 2) {
                cache[word] = entry;
                corrections.push_back({word, entry});
                break;
            }
        }
    }

    return corrections;
}

/**
 * Print the results of the spell check, including the misspelled words and
 * their suggested corrections.
 *
 * @param misspelled A vector of misspelled words.
 * @param corrections A vector of pairs, where each pair contains a
 * misspelled word and its suggested correction.
 */
void print_results(
    const std::vector<std::string>& misspelled,
    const std::vector<std::pair<std::string, std::string>>& corrections) {
    std::cout << "\n";

    if (misspelled.empty()) {
        std::cout << "No misspelled words found." << std::endl;
    } else {
        std::cout << "Misspelled words:" << std::endl;
        for (const auto& word : misspelled) {
            std::cout << word << std::endl;
        }
    }

    if (!corrections.empty()) {
        std::cout << "Corrections:" << std::endl;
        for (const auto& correction : corrections) {
            std::cout << correction.first << " -> " << correction.second
                      << std::endl;
        }
    }
}

/**
 * Removes punctuation from a given word and converts it to lowercase.
 * This function iterates through each character in the input word, checks
 * if it is an alphabetic character, and appends it to the result string
 * if true. This effectively strips the word of any punctuation.
 *
 * @param word The word from which to strip punctuation and convert to
 * lowercase.
 * @return A new string that is the lowercase version of the input word without
 * any punctuation.
 */
std::string strip_punctuation(const std::string& word) {
    std::string stripped_word;

    for (char c : word) {
        if (std::isalpha(c)) {
            stripped_word += std::tolower(c);
        }
    }

    return stripped_word;
}

/**
 * Tokenizes a given string of text into individual words. This function
 * uses a string stream to read individual words from the text string and
 * stores them in a vector. It handles standard whitespace-delimited words
 * but does not strip punctuation or alter case; use `strip_punctuation`
 * for that purpose if needed.
 *
 * @param text The string of text to tokenize.
 * @return A vector of strings, where each string is a word from the input text.
 */
std::vector<std::string> tokenize(const std::string& text) {
    std::vector<std::string> tokens;
    std::istringstream textStream(text);
    std::string token;

    while (textStream >> token) {
        // Check for trailing punctuation
        if (ispunct(token.back())) {
            // Separate word from trailing punctuation
            tokens.push_back(token.substr(0, token.size() - 1));
            tokens.push_back(token.substr(token.size() - 1));
        } else {
            tokens.push_back(token);
        }
    }

    return tokens;
}

/**
 * Replaces the first occurrence of a specified word in a given text with
 * a new word. This function searches for the old word within the original
 * text and replaces it with the new word if found. Note that this function
 * only replaces the first occurrence of the old word.
 *
 * @param original_text The original text in which to replace a word.
 * @param old_word The word in the original text that needs to be replaced.
 * @param new_word The word that will replace the old word in the text.
 * @return A new string with the first occurrence of the old word replaced by
 * the new word.
 */
std::string replace_word(const std::string& original_text,
                         const std::string& old_word,
                         const std::string& new_word) {
    std::string corrected_text = original_text;
    size_t pos = corrected_text.find(old_word);

    if (pos != std::string::npos) {
        corrected_text.replace(pos, old_word.length(), new_word);
    }

    return corrected_text;
}

/**
 * Reads a text file, identifies misspelled words, offers suggestions for
 * corrections, allows the user to choose corrections interactively, and
 * rewrites the file with the selected corrections applied. Ignores punctuation
 * when identifying words.
 *
 * @param filename The name of the file to spell check and correct.
 * @param dictionary A hash table containing the validated dictionary of words.
 */
void spell_check_and_correct_file(
    const std::string& filename,
    const std::unordered_map<std::string, bool>& dictionary) {
    std::ifstream file(filename);
    std::string text((std::istreambuf_iterator<char>(file)),
                     std::istreambuf_iterator<char>());
    file.close();

    std::unordered_set<std::string> dict_set;
    for (const auto& pair : dictionary) {
        dict_set.insert(pair.first);
    }

    auto tokens = tokenize(text);
    bool made_corrections = false;

    for (size_t i = 0; i < tokens.size(); ++i) {
        std::string strippedWord = strip_punctuation(tokens[i]);
        if (!strippedWord.empty() &&
            dict_set.find(strippedWord) == dict_set.end()) {
            // Misspelled word found
            std::cout << "\nMisspelled word: " << tokens[i] << std::endl;
            auto suggestions =
                suggest_corrections_cached({strippedWord}, dict_set);

            if (!suggestions.empty()) {
                // Display suggestions
                std::cout << "Suggestions for \"" << tokens[i]
                          << "\":" << std::endl;
                for (size_t j = 0; j < suggestions.size(); ++j) {
                    std::cout << j + 1 << ": " << suggestions[j].second
                              << std::endl;
                }
                std::cout << "0: Skip (make no change)\n";
                std::cout << "Choose a correction (number): ";
                int choice;
                std::cin >> choice;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
                                '\n');  // Clears the input buffer

                if (choice > 0 && choice <= suggestions.size()) {
                    // Replace the misspelled word with the chosen correction
                    tokens[i] = suggestions[choice - 1].second;
                    made_corrections = true;
                    std::cout << "Applying correction..." << std::endl;
                }
            }
        }
    }

    if (made_corrections) {
        // Reconstruct the text from tokens and write it back to the file,
        // handling spaces correctly
        std::string corrected_text;
        for (size_t i = 0; i < tokens.size(); ++i) {
            if (i > 0 && !ispunct(tokens[i][0])) {
                corrected_text += " ";
            }
            corrected_text += tokens[i];
        }

        std::ofstream out_file(filename);
        out_file << corrected_text;
        out_file.close();
        std::cout << "All corrections have been applied and saved back to \""
                  << filename << "\".\n";
    } else {
        std::cout << "No corrections were made to the file.\n";
    }
}

/**
 * Entry point of the program. Displays a UI to the user asking to input a
 * file name and a string of text to spell check. The program then reads the
 * dictionary from the file, checks the text for misspelled words, and
 * suggests corrections for any misspelled words found. The user can add new
 * words to the dictionary, and the hash table containing the dictionary
 * will be updated.
 */
int main() {
    std::unordered_map<std::string, bool> dictionary;
    std::string dictionary_filename, text, choice;

    while (true) {
        std::cout << "\n---- Spell Checker Menu ----\n"
                  << "[L] Load dictionary\n"
                  << "[C] Check spelling\n"
                  << "[F] Check spelling and correct file\n"
                  << "[A] Add word to dictionary\n"
                  << "[P] Purge cache\n"
                  << "[Q] Quit\n"
                  << "Choose an option: ";
        std::cin >> choice;

        // Clear input buffer
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == "L" || choice == "l") {
            std::cout << "\nEnter the name of the dictionary file: ";
            std::getline(std::cin, dictionary_filename);

            dictionary = load_dictionary(dictionary_filename);

            if (dictionary.empty()) {
                std::cerr << "\nFailed to load dictionary.\n";
            } else {
                std::cout << "\nDictionary loaded successfully.\n";
            }
        } else if (choice == "C" || choice == "c") {
            if (dictionary.empty()) {
                std::cout << "\nPlease load a dictionary first.\n";
                continue;
            }

            std::unordered_set<std::string> dict_set;
            for (const auto& pair : dictionary) {
                dict_set.insert(pair.first);
            }

            std::cout << "\nEnter the text to spell check:\n";
            std::getline(std::cin, text);

            auto misspelled = spell_check(text, dict_set);
            auto corrections = suggest_corrections_cached(misspelled, dict_set);

            print_results(misspelled, corrections);
        } else if (choice == "F" || choice == "f") {
            std::cout
                << "Enter the filename for spell checking and correction: ";
            std::string filename;
            std::getline(std::cin, filename);
            spell_check_and_correct_file(filename, dictionary);
        } else if (choice == "A" || choice == "a") {
            add_word_to_dictionary(dictionary);
        } else if (choice == "P" || choice == "p") {
            cache.clear();
            std::cout << "\nCache purged.\n";
        } else if (choice == "Q" || choice == "q") {
            std::cout << "\nExiting program.\n";
            break;
        } else {
            std::cout << "\nInvalid option. Please try again.\n";
        }
    }

    return 0;
}