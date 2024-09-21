#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define ALPHABET_SIZE 63
#define MAX_SIZE 100

typedef struct TreeNode {
    struct TreeNode **children;
    bool isEndOfkey;
    uint32_t value;
} TreeNode;

TreeNode *getNode(void) {
    TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));
    if (!node) {
        return NULL;
    }

    node->children = (TreeNode**)malloc(ALPHABET_SIZE * sizeof(TreeNode*));
    if (!node->children) {
        free(node);
        return NULL;
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        node->children[i] = NULL;
    }

    node->isEndOfkey = false;
    node->value = -1;
    return node;
}

int charToIndex(char c) {
    if ('a' <= c && c <= 'z') return c - 'a';
    if ('A' <= c && c <= 'Z') return 26 + (c - 'A');
    if ('0' <= c && c <= '9') return 52 + (c - '0');
    if (c == '_') return 62;
    return -1; 
}

char indexToChar(int index) {
    if (index >= 0 && index < 26) return 'a' + index;
    if (index >= 26 && index < 52) return 'A' + (index - 26);
    if (index >= 52 && index < 62) return '0' + (index - 52);
    if (index == 62) return '_';
    return '\0';
}

void insert(TreeNode *root, const char *key, const uint32_t value) {
    TreeNode *node = root;

    for (int i = 0; i < strlen(key); i++) {
        int index = charToIndex(key[i]);
        if (index == -1) {
            return;
        }
        if (!node->children[index]) {
            node->children[index] = getNode();
        }
        node = node->children[index];
    }
    node->isEndOfkey = true;
    node->value = value;
}

TreeNode* search(TreeNode *root, const char *key) {
    TreeNode *node = root;
    size_t key_length = strlen(key);

    for (int i = 0; i < key_length; i++) {
        int index = charToIndex(key[i]);
        if (index == -1) return NULL;
        if (!node->children[index]) return NULL;
        node = node->children[index];
    }
    if (node != NULL && node->isEndOfkey) {
        return node;
    }
    return NULL;
}

bool isLeafNode(TreeNode *node) {
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i]) {
            return false;
        }
    }
    return true;
}

bool deleteHelper(TreeNode *node, const char *key, int depth) {
    if (!node) {
        return false;
    }

    if (depth == strlen(key)) {
        if (node->isEndOfkey) {
            node->isEndOfkey = false;
            
            if (isLeafNode(node)) {
                return true;
            }
            return false;
        }
        return false;
    }

    int index = charToIndex(key[depth]);
    if (index != -1 && node->children[index]) {
        bool shouldDeleteChild = deleteHelper(node->children[index], key, depth+1);

        if (shouldDeleteChild) {
            free(node->children[index]);
            node->children[index] = NULL;
            
            if (!node->isEndOfkey && isLeafNode(node)) {
                return true;
            }
        }
    }
    return false;
}

void deleteKey(TreeNode *root, const char *key) {
    deleteHelper(root, key, 0);
}

void freeTree(TreeNode *node) {
    if (!node) return;

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i]) {
            freeTree(node->children[i]);
        }
    }

    free(node->children);
    free(node);
}


// int main() {
//     char keys[][11] = {"The_key", "the_key", "there_key", "any_key", "answer_key",  "by_key", "bye_key", "their_key"};
//     int values[] = {10, 20, 30, 40, 50, 60, 70, 80};
//     int n = sizeof(keys) / sizeof(keys[0]);

//     TreeNode *root = getNode();

//     for (int i = 0; i < n; i++) {
//         insert(root, keys[i], values[i]);
//     }

//     printf("Before deletion:\n");
//     printAllVariables(root);

//     deleteKey(root, "the_key");

//     printf("\nAfter deletion of 'the_key':\n");
//     printAllVariables(root);

//     // Free allocated memory
//     freeTree(root);
    
//     return 0;
// }
