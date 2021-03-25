/*

  Word Count using dedicated lists

*/

/*
Copyright © 2019 University of California, Berkeley

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include <assert.h>
#include <getopt.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

#include "word_count.h"

/* Global data structure tracking the words encountered */
WordCount* word_counts = NULL;

/* The maximum length of each word in a file */
#define MAX_WORD_LEN 64

/*
 * 3.1.1 Total Word Count
 *
 * Returns the total amount of words found in infile.
 * Useful functions: fgetc(), isalpha().
 */
int num_words(FILE* infile) {
    if (infile == NULL) {
        perror("Error opening the file");
        return -1;
    }

    int num_words = 0;
    int c;
    bool skip = 0; // skip word count as a non-alphabet encountered
    bool sep = 0; // separator character encountered

    while (!feof(infile))
    {
        c = fgetc(infile);

        if (c == ' ' || c == '\n') {
            if(!skip && !sep) {
                num_words++;
            }
            skip = 0;
            sep = 1;
        }
        else if (!isalpha(c)) {
            skip = 1;
        }
        else if (isalpha(c)) {
            sep = 0;
        }
    }

  return num_words;
}

/*
 * 3.1.2 Word Frequency Count
 *
 * Given infile, extracts and adds each word in the FILE to `wclist`.
 * Useful functions: fgetc(), isalpha(), tolower(), add_word().
 */
void count_words(WordCount** wclist, FILE* infile) {
    if (infile == NULL) {
        perror("Error opening the file");
    }
    

    int c;
    char* word = NULL;
    int wsize = 0;
    
    bool skip = 0; // skip word count as a non-alphabet encountered
    bool sep = 0; // separator character encountered

    while (!feof(infile))
    {
        c = fgetc(infile);

        if (c == ' ' || c == '\n') {
            if(!skip && !sep) {
                // printf("current word: %s\n", word);
                add_word(wclist, word);
            }
            skip = 0;
            sep = 1;
            wsize = 0;
            word = NULL;
        }
        else if (!isalpha(c)) {
            skip = 1;
            wsize = 0;
            word = NULL;
        }
        else if (isalpha(c)) {
            sep = 0;
            char lower_char = (char) tolower(c);
            
            wsize = wsize + sizeof(lower_char);

            word = (char *) realloc(word, wsize);
            strncat(word, &lower_char, wsize);
        }
    }
}

/*
 * Comparator to sort list by frequency.
 * Useful function: strcmp().
 */
static bool wordcount_less(const WordCount* wc1, const WordCount* wc2) {
    if (wc1->count != wc2->count) {
        // If wc1 count is less than wc2 count. wc1 comes before wc2.
        // If wc2 count is less than wc1 count. wc2 comes before wc1.
        return wc1->count < wc2->count;
    } else if (wc1->count == wc2->count) { // sort alphabetically
        int strret = strcmp(wc1->word, wc2->word); // string return

        if (strret <= 0) {
            return 1; // wc1 < wc2
        } else if (strret > 0)
        {
            return 0; // wc2 < wc1
        }
        
        // Custom strcmp
        // int idx = 0;
        // int wc1_len = (int) strlen(wc1->word);
        // int wc2_len = (int) strlen(wc2->word);
        
        // for (; idx < wc1_len && idx < wc2_len; idx++) {
        //     if (wc1->word[idx] != wc2->word[idx]) {
        //         return wc1->word[idx] < wc2->word[idx];
        //     }
        // }

        // if (idx == wc1_len) {
        //     return 1; // wc1 < wc2
        // }

        // if (idx == wc2_len) {
        //     return 0; // wc2 < wc1
        // }

        // return wc1->word[0] < wc2->word[0];
    }
    return 1;
    }

// In trying times, displays a helpful message.
static int display_help(void) {
    printf("Flags:\n"
            "--count (-c): Count the total amount of words in the file, or STDIN if a file is not "
            "specified. This is default behavior if no flag is specified.\n"
            "--frequency (-f): Count the frequency of each word in the file, or STDIN if a file is "
            "not specified.\n"
            "--help (-h): Displays this help message.\n");
    return 0;
}

/*
 * Handle command line flags and arguments.
 */
int main(int argc, char* argv[]) {

    // Count Mode (default): outputs the total amount of words counted
    bool count_mode = true;
    int total_words = 0;

    // Freq Mode: outputs the frequency of each word
    bool freq_mode = false;

    FILE* infile = NULL;

    // Variables for command line argument parsing
    int i;
    static struct option long_options[] = {{"count", no_argument, 0, 'c'},
                                            {"frequency", no_argument, 0, 'f'},
                                            {"help", no_argument, 0, 'h'},
                                            {0, 0, 0, 0}};

    // Sets flags
    while ((i = getopt_long(argc, argv, "cfh", long_options, NULL)) != -1) {
        switch (i) {
            case 'c':
                count_mode = true;
                freq_mode = false;
                break;
            case 'f':
                count_mode = false;
                freq_mode = true;
                break;
            case 'h':
                return display_help();
        }
    }

    if (!count_mode && !freq_mode) {
        printf("Please specify a mode.\n");
        return display_help();
    }

    /* Create the empty data structure */
    init_words(&word_counts);

    if ((argc - optind) < 1) {
        // No input file specified, instead, read from STDIN instead.
        infile = fopen("words.txt", "r");
    } else {
        // At least one file specified. Useful functions: fopen(), fclose().
        // The first file can be found at argv[optind]. The last file can be
        // found at argv[argc-1].
        infile = fopen(argv[optind], "r");
    }

    if (count_mode) {
        total_words = num_words(infile);
        printf("The total number of words is: %i\n", total_words);
    } else {
        count_words(&word_counts, infile);
        wordcount_sort(&word_counts, wordcount_less);

        printf("The frequencies of each word are: \n");
        fprint_words(word_counts, stdout);

        // free memory (word_counts)
    }

    return 0;
}
