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

typedef struct AVLNode {
    char *token;
    char *encoding;
    struct AVLNode *left;
    struct AVLNode *right;
    int height;
} AVLNode;

// Priority queue ADT functions
static struct priority_queue *priority_queue_new();
static void priority_queue_free(struct priority_queue *queue);
static int priority_queue_size(struct priority_queue *queue);
static void priority_queue_insert(struct priority_queue *queue, struct huffmanTree *value);
static struct huffmanTree *priority_queue_remove(struct priority_queue *queue);

// Task 3 Functions to create a Huffman tree
static struct huffmanTree *createLeaf(struct item item);
static struct huffmanTree *createNode(struct huffmanTree *left, struct huffmanTree *right);

// AVL Tree Mapping Functions
static AVLNode* AVLTreeMapping(struct huffmanTree *tree);
static AVLNode* insertAVL(AVLNode* node, char *token, char *encoding);
static void traverseHuffman(struct huffmanTree *node, char *path, AVLNode **avlRoot);
static int height(AVLNode *node);
static int getBalance(AVLNode *node);
static AVLNode* rightRotate(AVLNode *y);
static AVLNode* leftRotate(AVLNode *x);
static int max(int a, int b);
static void freeAVLTree(AVLNode *root);

// Task 4


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
    // Open the input file
    File inputFile = FileOpenToRead(inputFilename);

    // Create an AVL tree to store the mapping from tokens to encodings
    AVLNode *avlRoot = AVLTreeMapping(tree);

    // Create a string to store the encoded string
    char *outputString = malloc(1 * sizeof(char));
    outputString[0] = '\0';

    char token[MAX_TOKEN_LEN];
    while ((FileReadToken(inputFile, token))) {
        // Find the encoding for the current token 
        // by post-order traversal of the AVL tree
        AVLNode *curr = avlRoot;
        while (curr != NULL && strcmp(token, curr->token) != 0) {
            if (strcmp(token, curr->token) < 0) {
                curr = curr->left;
            } else {
                curr = curr->right;
            }
        }
        if (curr == NULL) {
            fprintf(stderr, "Error: token not found in AVL tree\n");
            exit(EXIT_FAILURE);
        }
        char *encoding = strdup(curr->encoding);
        //printf("%s -> %s\n", token, encoding);

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
    freeAVLTree(avlRoot);
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

//// My AVL Tree Functions ////
// Helper function to get the height of an AVL node
int height(AVLNode *node) {
    if (node == NULL) return 0;
    return node->height;
}

// Helper function to get the balance factor of an AVL node
int getBalance(AVLNode *node) {
    if (node == NULL) return 0;
    return height(node->left) - height(node->right);
}

// Right rotate function
AVLNode* rightRotate(AVLNode *y) {
    AVLNode *x = y->left;
    AVLNode *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = 1 + max(height(y->left), height(y->right));
    x->height = 1 + max(height(x->left), height(x->right));

    return x;
}

// Left rotate function
AVLNode* leftRotate(AVLNode *x) {
    AVLNode *y = x->right;
    AVLNode *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = 1 + max(height(x->left), height(x->right));
    y->height = 1 + max(height(y->left), height(y->right));

    return y;
}

// Insert function for AVL tree
AVLNode* insertAVL(AVLNode* node, char *token, char *encoding) {
    if (node == NULL) {
        AVLNode* newNode = (AVLNode*)malloc(sizeof(AVLNode));
        newNode->token = strdup(token);
        newNode->encoding = strdup(encoding);
        newNode->left = newNode->right = NULL;
        newNode->height = 1;
        return newNode;
    }

    // Insertion logic here (similar to BST)
    if (strcmp(token, node->token) < 0) {
        node->left = insertAVL(node->left, token, encoding);
    } else if (strcmp(token, node->token) > 0) {
        node->right = insertAVL(node->right, token, encoding);
    } else {
        return node; // Duplicates are not allowed
    }

    // Update height
    node->height = 1 + max(height(node->left), height(node->right));

    // Check for imbalance and perform rotations
    int balance = getBalance(node);

    if (balance > 1 && strcmp(token, node->left->token) < 0) {
        return rightRotate(node);
    }

    if (balance < -1 && strcmp(token, node->right->token) > 0) {
        return leftRotate(node);
    }

    if (balance > 1 && strcmp(token, node->left->token) > 0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && strcmp(token, node->right->token) < 0) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

// Recursive function to traverse the Huffman tree and store paths in AVL tree
void traverseHuffman(struct huffmanTree *node, char *path, AVLNode **avlRoot) {
    if (node == NULL) return;

    if (node->token != NULL) {
        *avlRoot = insertAVL(*avlRoot, node->token, path);
    }

    char leftPath[1000], rightPath[1000];
    strcpy(leftPath, path);
    strcpy(rightPath, path);
    strcat(leftPath, "0");
    strcat(rightPath, "1");

    traverseHuffman(node->left, leftPath, avlRoot);
    traverseHuffman(node->right, rightPath, avlRoot);
}

// Main function to create the AVL tree mapping from Huffman tree
static AVLNode* AVLTreeMapping(struct huffmanTree *tree) {
    AVLNode *avlRoot = NULL;
    traverseHuffman(tree, "", &avlRoot);
    return avlRoot;
}

static void freeAVLTree(AVLNode *root) {
    if (root == NULL) return; // Base case

    // Recursively free left and right subtrees
    freeAVLTree(root->left);
    freeAVLTree(root->right);

    // Free dynamically allocated data within the node
    free(root->token);
    free(root->encoding);

    // Finally, free the node itself
    free(root);
}

// Helper function to get the maximum of two integers
static int max(int a, int b) {
    return (a > b) ? a : b;
}