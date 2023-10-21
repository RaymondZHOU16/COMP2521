// Implementation of the Huffman module

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Counter.h"
#include "File.h"
#include "huffman.h"

struct priority_queue {
    struct huffmanTree **data;
    int size;
    int capacity;
};

typedef struct Stack {
    char *data;
    int top;
    int capacity;
} Stack;

static struct huffmanTree *createLeaf(struct item item);
static struct huffmanTree *createNode(struct huffmanTree *left, struct huffmanTree *right);
static char *findingEncoding(struct huffmanTree *tree, char *token);
static struct priority_queue *priority_queue_new();
static void priority_queue_free(struct priority_queue *queue);
static int priority_queue_size(struct priority_queue *queue);
static void priority_queue_insert(struct priority_queue *queue, struct huffmanTree *value);
static struct huffmanTree *priority_queue_remove(struct priority_queue *queue);
static Stack* createStack();
static void resizeStack(Stack *stack);
static void push(Stack *stack, char value);
static char pop(Stack *stack);
static char* stackToString(Stack *stack);
static int findToken(struct huffmanTree *tree, char *token, Stack *stack, char **result);

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

    // Traverse the tree using the encoding
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
    // Open the input file
    File inputFile = FileOpenToRead(inputFilename);

    // Create a counter to count the occurrences of each token
    Counter counter = CounterNew();

    // Count the frequency of each token in the input file
    char token[MAX_TOKEN_LEN];
    while ((FileReadToken(inputFile, token))) {
        CounterAdd(counter, token);
    }

    // Get an array of items from the counter
    int numItems;
    struct item *items = CounterItems(counter, &numItems);

    // Create a priority queue of leaf nodes
    struct priority_queue *queue = priority_queue_new();
    for (int i = 0; i < numItems; i++) {
        struct huffmanTree *leaf = createLeaf(items[i]);
        priority_queue_insert(queue, leaf);
    }

    // Construct the Huffman tree by repeatedly merging the two lowest-frequency nodes
    while (priority_queue_size(queue) > 1) {
        // Remove the two lowest-frequency nodes from the priority queue
        struct huffmanTree *node1 = priority_queue_remove(queue);
        struct huffmanTree *node2 = priority_queue_remove(queue);

        // Create a new node that merges the two lowest-frequency nodes
        struct huffmanTree *newNode = createNode(node1, node2);

        // Add the new node to the priority queue
        priority_queue_insert(queue, newNode);
    }

    // The root of the Huffman tree is the only node left in the priority queue
    struct huffmanTree *root = priority_queue_remove(queue);

    // Free ALL
    priority_queue_free(queue);
    CounterFree(counter);
    for (int i = 0; i < numItems; i++) {
        free(items[i].token);
    }
    free(items);

    // Close the input file
    FileClose(inputFile);

    // Return the root of the Huffman tree
    return root;
}

// Task 4
char *encode(struct huffmanTree *tree, char *inputFilename) {
	// Open to read the input file
    File inputFile = FileOpenToRead(inputFilename);
    
    // Create a string to store the encoded string
    char *outputString = malloc(1 * sizeof(char));
    outputString[0] = '\0';

    char token[MAX_TOKEN_LEN];
    while ((FileReadToken(inputFile, token))) {
        // Find the encoding for the current token
        char *encoding = findingEncoding(tree, token);
        // printf("Token: %s; Encoding: %s\n", token, encoding);

        // Append the encoding to the output string
        size_t newLength = strlen(outputString) + strlen(encoding) + 2;
        outputString = realloc(outputString, newLength);
        strcat(outputString, encoding);

        // Free the encoding string
        free(encoding);
    }

    // Append the null terminator to the output string
    outputString = realloc(outputString, strlen(outputString) + 1);
    strcat(outputString, "\0");

    // Close the input file and return the encoded string
    FileClose(inputFile);
    return outputString;
}

//////////////////////
//// My Functions ////
//////////////////////

// Create a new leaf node for the given token and frequency
static struct huffmanTree *createLeaf(struct item item) {
    struct huffmanTree *leaf = malloc(sizeof(struct huffmanTree));
    if (leaf == NULL) {
        fprintf(stderr, "Error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    leaf->token = strdup(item.token);
    leaf->freq = item.freq;
    leaf->left = NULL;
    leaf->right = NULL;
    return leaf;
}

// Create a new node that merges the two given nodes
static struct huffmanTree *createNode(struct huffmanTree *left, struct huffmanTree *right) {
    struct huffmanTree *node = malloc(sizeof(struct huffmanTree));
    if (node == NULL) {
        fprintf(stderr, "Error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    node->token = NULL;
    node->freq = left->freq + right->freq;
    node->left = left;
    node->right = right;
    return node;
}

static char* stackToString(Stack *stack) {
    char *result = (char *)malloc((stack->top + 2) * sizeof(char));
    strncpy(result, stack->data, stack->top + 1);
    result[stack->top + 1] = '\0';
    return result;
}

// Recursive function to find the Huffman encoding
static int findToken(struct huffmanTree *tree, char *token, Stack *stack, char **result) {
    if (!tree) return 0;

    if (tree->token && strcmp(tree->token, token) == 0) {
        *result = stackToString(stack);
        return 1;
    }

    push(stack, '0');
    if (findToken(tree->left, token, stack, result)) return 1;
    pop(stack);

    push(stack, '1');
    if (findToken(tree->right, token, stack, result)) return 1;
    pop(stack);

    return 0;
}

static char* findingEncoding(struct huffmanTree *tree, char *token) {
    Stack *stack = createStack();
    char *result = NULL;
    findToken(tree, token, stack, &result);
    free(stack->data);
    free(stack);
    return result;
}

//// My Priority Queue Functions ////
// Create a new internal node for the current item
static struct priority_queue *priority_queue_new() {
    struct priority_queue *queue = malloc(sizeof(struct priority_queue));
    queue->data = malloc(16 * sizeof(struct huffmanTree *));
    queue->size = 0;
    queue->capacity = 16;
    return queue;
}

// Free the memory allocated to the priority queue
static void priority_queue_free(struct priority_queue *queue) {
    free(queue->data);
    free(queue);
}

// Return the number of items in the priority queue
static int priority_queue_size(struct priority_queue *queue) {
    return queue->size;
}

// Insert a new item into the priority queue
static void priority_queue_insert(struct priority_queue *queue, struct huffmanTree *value) {
    if (queue->size == queue->capacity) {
        queue->capacity *= 2;
        queue->data = realloc(queue->data, queue->capacity * sizeof(struct huffmanTree *));
    }
    queue->data[queue->size] = value;
    int i = queue->size;
    while (i > 0 && queue->data[i]->freq < queue->data[(i-1)/2]->freq) {
        struct huffmanTree *temp = queue->data[i];
        queue->data[i] = queue->data[(i-1)/2];
        queue->data[(i-1)/2] = temp;
        i = (i-1)/2;
    }
    queue->size++;
}

// Remove and return the item with the lowest frequency from the priority queue
static struct huffmanTree *priority_queue_remove(struct priority_queue *queue) {
    if (queue->size == 0) {
        return NULL;
    }
    struct huffmanTree *result = queue->data[0];
    queue->size--;
    queue->data[0] = queue->data[queue->size];
    int i = 0;
    while (2*i+1 < queue->size) {
        int j = 2*i+1;
        if (j+1 < queue->size && queue->data[j+1]->freq < queue->data[j]->freq) {
            j++;
        }
        if (queue->data[j]->freq < queue->data[i]->freq) {
            struct huffmanTree *temp = queue->data[i];
            queue->data[i] = queue->data[j];
            queue->data[j] = temp;
            i = j;
        } else {
            break;
        }
    }
    return result;
}

//// My Stack Functions ////
static Stack* createStack() {
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    stack->capacity = 10; // Initial capacity
    stack->top = -1;
    stack->data = (char *)malloc(stack->capacity * sizeof(char));
    return stack;
}

static void resizeStack(Stack *stack) {
    stack->capacity *= 2;
    stack->data = (char *)realloc(stack->data, stack->capacity * sizeof(char));
}

static void push(Stack *stack, char value) {
    if (stack->top == stack->capacity - 1) {
        resizeStack(stack);
    }
    stack->data[++stack->top] = value;
}

static char pop(Stack *stack) {
    if (stack->top == -1) return '\0'; // Stack underflow
    return stack->data[stack->top--];
}