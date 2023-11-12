
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "HashTable.h"

bool areSimilarStrings(char *s1, char *s2);

int main(int argc, char *argv[]) {
	if (argc != 3) {
		fprintf(stderr, "usage: %s <string 1> <string 2>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	bool result = areSimilarStrings(argv[1], argv[2]);
	printf("The strings %s similar\n", result ? "are" : "are not");
	return EXIT_SUCCESS;
}

// !!! DO NOT MODIFY THE CODE ABOVE !!!
////////////////////////////////////////////////////////////////////////
// Your task

/**
 * Returns true if two strings are similar, and false otherwise. Two strings s1
 * and s2 are similar if, for each character in s1, it is possible to *uniquely*
 * replace it by another character (possibly itself) such that, after all
 * replacements are done to s1, we end up with s2. For example, the strings
 * "adt" and "bst" are similar, but "adt" and "dcc" are not.
 */
bool areSimilarStrings(char *s1, char *s2) {
	// TODO: Complete this function!
    // Check if lengths of s1 and s2 are the same
    int len1 = strlen(s1);
    int len2 = strlen(s2);
    if (len1 != len2) {
        return false;
    }

    HashTable charMap = HashTableNew();
    HashTable reverseMap = HashTableNew();

    for (int i = 0; i < len1; i++) {
        char from = s1[i];
        char to = s2[i];

        if (HashTableContains(charMap, from)) {
            // If 'from' is already mapped to a different character, return false
            if (HashTableGet(charMap, from) != to) {
                HashTableFree(charMap);
                HashTableFree(reverseMap);
                return false;
            }
        } else {
            // If 'to' is already mapped from a different character, return false
            if (HashTableContains(reverseMap, to)) {
                HashTableFree(charMap);
                HashTableFree(reverseMap);
                return false;
            }

            // Add the mapping
            HashTableInsert(charMap, from, to);
            HashTableInsert(reverseMap, to, from);
        }
    }

    HashTableFree(charMap);
    HashTableFree(reverseMap);
    return true;
}

