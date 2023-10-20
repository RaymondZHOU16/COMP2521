// Implementation of the Huffman module

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Counter.h"
#include "File.h"
#include "huffman.h"

// Task 1
void decode(struct huffmanTree *tree, char *encoding, char *outputFilename) {
    // Check if any of the input parameters are NULL
    if (tree == NULL || encoding == NULL || outputFilename == NULL) {
        fprintf(stderr, "Invalid input parameters\n");
		exit(EXIT_FAILURE);		
    }

    // Open the output file for writing
    FILE *outputFile = fopen(outputFilename, "w");
    if (outputFile == NULL) {
        fprintf(stderr, "Error opening output file");
        exit(EXIT_FAILURE);
    }

    struct huffmanTree *curr = tree;
    for (int n = 0; encoding[n] != '\0'; n++) {		
        if (encoding[n] == '0') {
			curr = curr->left;
		} else if (encoding[n] == '1') {
			curr = curr->right;
		} else {
			fprintf(stderr, "error: Invaild Character '%c' of encoding\n ", encoding[n]);
			exit(EXIT_FAILURE);
		}
		if (curr->left == NULL && curr->right == NULL) {
			fprintf(outputFile, "%s", curr->token);
			curr = tree;
		}
    }

	// Close the output file when done
    fclose(outputFile);
}

// Task 3
struct huffmanTree *createHuffmanTree(char *inputFilename) {
	return NULL;
}

// Task 4
char *encode(struct huffmanTree *tree, char *inputFilename) {
	return NULL;
}

