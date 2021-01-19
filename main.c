#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// Change this to 1 if you want to type a file for yourself
#define ENTERTEXT 0

typedef struct Words {
    uint16_t character_count;
    uint16_t digit_count;
    uint16_t w_counter;
    uint16_t symbol_count;
    char the_word[255];
} Word;

void enter_text ();
void count_occurance ();
void graph_biggest_word (Word *words, uint16_t unique_words);
void word_count (Word *word, uint16_t word_counter, uint16_t unique_words);
void symbol_count (Word *word, uint16_t unique_words);
void graph_biggest_word_horizontal (Word *words, uint16_t unique_words);

int main()
{
    // Writing the text if we do not have one:
    enter_text ();

    FILE *file_pointer;
    char buff [255];
    uint16_t word_counter = 0;

    // Open the file
    file_pointer = fopen ("Temp.txt", "r");

    // Count how many words there are
    while (fscanf (file_pointer, "%s", buff) != EOF) {
        ++word_counter;
    }

    // Close the file
    fclose (file_pointer);

    // Create the array of words. This is wasteful of memory, but the dynamic way of resizing arrays is too complex.
    Word *words;
    words = (struct Word*) malloc (word_counter * sizeof(Word));

    // Initialize the elements
    uint16_t k = 0;

    for (k = 0; k < word_counter; ++k) {
        words[k].character_count = 0;
        words[k].digit_count = 0;
        words[k].symbol_count = 0;
        words[k].w_counter = 1;
    }

    // Open the file
    file_pointer = fopen ("Temp.txt", "r");

    // Populate the array with all the words
    uint16_t unique_words = 1;

    while (fscanf (file_pointer, "%s", buff) != EOF) {
        // Count word occurance
        uint16_t j = 0;
        uint16_t match_counter = 0;

        // Look through the dictionary
        for (j = 0; j < unique_words; ++j) {

            if ((strcmp(words[j].the_word, buff) == 0)) {
                ++words[j].w_counter;
                ++match_counter;
            }
        }

        if (match_counter == 0) {
            // New word detected
            // The word is saved in the dictionary
            strcpy (words[j - 1].the_word, buff);

            // Sort through the symbols
            // IMPORTANT: There is no criteria about Upper case and Lower case letters. Therefore, upper case letter
            // words are treated as different words than their lower case counterparts.
            uint8_t index = 0;
            char c;
            while ((c = words[unique_words - 1].the_word[index]) != '\0')  {
                if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
                    ++words[unique_words - 1].character_count;
                } else if (c >= '0' && c <= '9') {
                    ++words[unique_words - 1].digit_count;
                } else {
                    ++words[unique_words - 1].symbol_count;
                }
                ++index;
            }
            ++unique_words;
        }
    }

    // Plot the graph
    graph_biggest_word_horizontal (words, unique_words - 1);

    // Word count + word frequency
    word_count (words, word_counter, unique_words - 1);

    // Symbol count, letter count, spaces count, others count (special symbols and digits)
    symbol_count (words, unique_words - 1);

    return 0;
}

void enter_text ()
{
    // create a file with your text here
    if (ENTERTEXT) {
        FILE *fp;
        char sentence [65535];

        // Write your book here
        fp = fopen ("Temp.txt", "w+");
        fgets(sentence, sizeof(sentence), stdin);
        fprintf(fp, "%s", sentence);
        fclose (fp);
    } else {
        printf ("Enter your text here:\n");
        printf ("I have one prepared:\n\n");

        // Read the loaded file
        FILE *fp;
        char c;

        fp = fopen ("Temp.txt", "r");
        while ((c = fgetc (fp)) != EOF) {
            printf ("%c", c);
        }
        fclose (fp);
        printf ("\n\n");
    }
}


void graph_biggest_word (Word *words, uint16_t unique_words)
{
    // Numbers are not words. Also symbols of any kind do not count
    // Find the longest word
    uint16_t i = 0;
    uint16_t lengthiest = 0;
    uint16_t longest_word_index = 0;

    for (i = 0; i < unique_words; ++i) {
        if (lengthiest < words[i].character_count) {
            lengthiest = words[i].character_count;
            longest_word_index = i;
        }
    }

    // Finding the percentages
    uint8_t percentage_array[unique_words];

    for (i = 0; i < unique_words; ++i) {
        // Make it 10% for one bar, so divide by 10
        percentage_array[i] = words[i].character_count * 10 / lengthiest;
    }

    // Printing the graph
    uint8_t j = 0;
    uint8_t k = 0;


    for (i = 0; i < 10; ++i) {
        for (j = 0; j < unique_words; ++j) {
            // Check for actual words
            if (words[j].character_count !=0) {
                // If it is even or not
                if (words[j].character_count % 2 && words[j].character_count != 1) {
                    for (k = 0; k < words[j].character_count / 2; ++k) {
                        printf (" ");
                    }
                    if (10 - percentage_array[j] <= i) {
                        printf ("|");
                    } else {
                        printf (" ");
                    }
                    for (k = 0; k < words[j].character_count / 2; ++k) {
                        printf (" ");
                    }
                } else {
                    for (k = 0; k < words[j].character_count / 2; ++k) {
                        printf (" ");
                    }
                    if (10 - percentage_array[j] <= i) {
                        printf ("|");
                    } else {
                        printf (" ");
                    }
                    for (k = 0; k < words[j].character_count / 2 - 1; ++k) {
                        printf (" ");
                    }
                }
                printf(" ");
            }
        }
        printf ("\n");
    }

    // Cleaning up and printing the words
    char c;

    for (i = 0; i < unique_words; ++i) {
        j = 0;
        while ((c = words[i].the_word[j++]) != '\0') {
            if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
                printf ("%c", c);
            }
        }
        if (words[i].character_count != 0) {
            printf (";", words[i].the_word);
        }
    }
    printf ("\n\n");
}

void word_count (Word *words, uint16_t word_counter, uint16_t unique_words)
{
    // Print how many words there are in the text
    printf ("The total number of words in the text is: %d\n", word_counter);
    printf ("The number of unique words in the text is: %d\n", unique_words);

    // Find out what is the most used word
    uint16_t i = 0;
    uint16_t frequency = 1;
    Word the_word;
    strcpy (the_word.the_word, words[0].the_word);

    for (i = 0; i < unique_words; ++i) {
        if (frequency < words[i].w_counter) {
            strcpy (the_word.the_word, words[i].the_word);
            frequency = words[i].w_counter;
        }
    }

    // Print the most used word and how many times it has been encountered
    printf ("The most used word in the text is \"%s\". It is used %d times.\n", the_word.the_word, frequency);
}

void symbol_count (Word *word, uint16_t unique_words)
{
    uint32_t total_character_count = 0;
    uint32_t total_digits_count = 0;
    uint32_t total_special_symbols_count = 0;
    uint32_t total_symbol_count = 0;
    uint32_t total_interval_count = 0;
    uint16_t i = 0;

    // Count all the things in the universe!
    for (i = 0; i < unique_words; ++i) {
        total_character_count += word[i].character_count;
        total_digits_count += word[i].digit_count;
        total_special_symbols_count += word[i].symbol_count;
    }

    FILE *fp;
    char c;

    fp = fopen ("Temp.txt", "r");
    while ((c = fgetc (fp)) != EOF) {
        if (c == ' ') {
            ++total_interval_count;
        }
    }
    fclose (fp);

    total_symbol_count = total_character_count + total_digits_count + total_special_symbols_count;
    printf ("The total count of symbols in the text is: %d\n", total_symbol_count);
    printf ("The total count of characters in the text is: %d\n", total_character_count);
    printf ("The total number of intervals in the text is: %d\n", total_interval_count);
    printf ("The total count of digits in the text is: %d\n", total_digits_count);
    printf ("The total count of special symbols in the text is: %d\n", total_special_symbols_count);
}

// Bonus for me
void graph_biggest_word_horizontal (Word *words, uint16_t unique_words)
{
    // Numbers are not words. Also symbols of any kind do not count
    // Find the longest word
    uint16_t i = 0;
    uint16_t lengthiest = 0;
    uint16_t longest_word_index = 0;

    for (i = 0; i < unique_words; ++i) {
        if (lengthiest < words[i].character_count) {
            lengthiest = words[i].character_count;
            longest_word_index = i;
        }
    }

    // Finding the percentages
    uint8_t percentage_array[unique_words];

    for (i = 0; i < unique_words; ++i) {
        // Make it 10% for one bar, so divide by 10
        percentage_array[i] = words[i].character_count * 10 / lengthiest;
    }

    // Printing the graph
    uint8_t j = 0;
    char c;

    for (i = 0; i < unique_words; ++i) {
        j = 0;
        if (words[i].character_count != 0) {
            while ((c = words[i].the_word[j++]) != '\0') {
                if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
                    printf ("%c", c);
                }
            }
            printf (":");
            for (j = 0; j < percentage_array[i]; ++j) {
                printf ("-");
            }
            printf ("\n");
        }
    }
    printf ("\n\n");
}
