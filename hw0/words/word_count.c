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

word_count provides lists of words and associated count

Functional methods take the head of a list as first arg.
Mutators take a reference to a list as first arg.
*/

#include "word_count.h"

/* Basic utililties */

char* new_string(char* str) { return strcpy((char*)malloc(strlen(str) + 1), str); }

void init_words(WordCount** wclist) {
    /* Initialize word count.  */
    *wclist = NULL;
}

size_t len_words(WordCount* wchead) {
    WordCount* wc = wchead;
    size_t len = 0;
    
    while (wc->next) {
        len++;
        wc = wc->next;
    }
    
    return len;
}

WordCount* find_word(WordCount* wchead, char* word) {
    /* Return WordCount pointer of word, if it exists */
    
    WordCount* wc_null = NULL;
    WordCount* wc = wchead;

    if (!wchead) { return wc_null; }
    
    while (wc->next) {
        if (strcmp(wc->word, word) == 0)
        {
            return wc;
        }
        wc = wc->next;
    }
    
    return wc_null;
}

void add_word(WordCount** wclist, char* word) {
    /* If word is present in word_counts list, increment the count, otw insert with count 1. */
    // printf("add word called\n");
    // printf("wclist address: %p\n", wclist);
    char* word_copy;
    word_copy = (char*) malloc(strlen(word));
    strcpy(word_copy, word);

    // WordCount* next_word = &(WordCount){.word = word_copy, .count = 1, .next = NULL}; // compund literal
    WordCount* next_word = malloc(sizeof(WordCount));
    next_word->word = word_copy;
    next_word->count = 1;
    next_word->next = NULL;

    // printf("incoming word is: %s, ptr: %p\n", word, word);
    // printf("incoming word copy is: %s, ptr: %p\n", word_copy, word_copy);
    // printf("wclist is: %p, wchead is: %p\n", wclist, *wclist);
    // printf("Next word is: %p\n", next_word->word);

    // printf("find the word: %p\n", find_word(*wclist, word));

    if (!*wclist) {
        // printf("no word\n");
        *wclist = next_word;
    } else {
        WordCount* word_found = find_word(*wclist, word);

        if (!word_found) {
            WordCount* ptr = *wclist;
            while (ptr->next) {
                ptr = ptr->next;
            }
            ptr->next = next_word;
        } else {
            word_found->count++;
            // printf("%s word found. Count increased to %d\n", word_found->word, word_found->count);
        }
    }

    // printf("wclist is: %p, wchead is: %p\n", wclist, *wclist);
    // printf("size of wchead is: %lu\n", sizeof(**wclist));
    // printf("string is: %s, ptr: %p\n", (*wclist)->word, (*wclist)->word);
    // printf("count is: %d\n", (*wclist)->count);
    // printf("next is: %p\n", (*wclist)->next);

    // fprint_words(*wclist, stdout);
    
    // printf("\n");
}

void fprint_words(WordCount* wchead, FILE* ofile) {
    /* print word counts to a file */
    WordCount* wc;
    for (wc = wchead; wc; wc = wc->next) {
        fprintf(ofile, "%i\t%s\n", wc->count, wc->word);
    }
}
