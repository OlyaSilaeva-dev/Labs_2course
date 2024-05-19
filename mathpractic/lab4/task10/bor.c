#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define ALPHABET_SIZE 37
#define MAX_SIZE 100 

typedef struct TreeNode {
    struct TreeNode **children;
    bool isEndOfWord;
    uint32_t value;
} TreeNode;

TreeNode *getNode(void) {
    TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));
    if(!node) {
        return NULL;
    }

    node->children = (TreeNode**)malloc(ALPHABET_SIZE * sizeof(TreeNode*));
    if(!node->children) {
        free(node);
        return NULL;
    }

    for(int i = 0; i < ALPHABET_SIZE; i++) {
        node->children[i] = NULL;
    }

    node->isEndOfWord = false;
    node->value = -1;
    return node;
}

int charToIndex(char c) {
    if ('a' <= c && c <= 'z') return c - 'a';
    if ('0' <= c && c <= '9') return 26 + (c - '0');
    if (c == '_') return 36;
    return -1; 
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
    node->isEndOfWord = true;
    node->value = value;
}

TreeNode* search(TreeNode *root, const char *key) {
    TreeNode *node = root;
    for (int i = 0; i < strlen(key); i++) {
        int index = charToIndex(key[i]);
        if (index == -1) return NULL;
        if (!node->children[index]) return NULL;
        node = node->children[index];
    }
    if (node != NULL && node->isEndOfWord) {
        return node;
    }
    return NULL;
}

bool isLeafNode(TreeNode *node) {
    return node->isEndOfWord != false;
}

bool deleteHelper(TreeNode *node, const char *key, int depth) {
    if (!node) return false;

    if (depth == strlen(key)) {
        if (node->isEndOfWord) {
            node->isEndOfWord = false;
        }

        bool hasNoChildren = true;
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if (node->children[i]) {
                hasNoChildren = false;
                break;
            }
        }

        return hasNoChildren;
    }

    int index = charToIndex(key[depth]);
    if (deleteHelper(node->children[index], key, depth + 1)) {
        free(node->children[index]);
        node->children[index] = NULL;

        bool hasNoChildren = true;
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if (node->children[i]) {
                hasNoChildren = false;
                break;
            }
        }

        return !node->isEndOfWord && hasNoChildren;
    }
    return false;
}

void deleteKey(struct TreeNode *root, const char *key) {
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

void printAllVariablesHelper(TreeNode *node, char *buffer, int depth) {
    if (node == NULL) return;

    if (node->isEndOfWord) {
        buffer[depth] = '\0';
        printf("%s: %u\n", buffer, node->value);
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i]) {
            if (i < 26) {
                buffer[depth] = 'a' + i;
            } else if (i < 36) {
                buffer[depth] = '0' + (i - 26);
            } else {
                buffer[depth] = '_';
            }
            printAllVariablesHelper(node->children[i], buffer, depth + 1);
        }
    }
}

void printAllVariables(TreeNode *root) {
    char buffer[MAX_SIZE];  // Максимальная длина ключа 100 символов
    printAllVariablesHelper(root, buffer, 0);
}
// int main() {
//     char keys[][10] = {"the_key", "a_key", "there_key", "answer_key", "any_key", "by_key", "bye_key", "their_key"};
//     int values[] = {10, 20, 30, 40, 50, 60, 70, 80}; // Значения, ассоциированные с ключами
//     int n = sizeof(keys) / sizeof(keys[0]);

//     TreeNode *root = getNode();

//     for (int i = 0; i < n; i++) {
//         insert(root, keys[i], values[i]);
//     }

//     for (int i = 0; i < n; i++) {
//         int val = search(root, keys[i]);
//         val != -1 ? printf("%s --- find %d\n", keys[i], val) : printf("%s --- no find\n", keys[i]);
//     }

//     deleteKey(root, "the_key");
//     int val = search(root, "the_key");
//     val != -1 ? printf("the_key --- find %d\n", val) : printf("the_key --- no find\n");

//     freeTree(root);
//     return 0;
// }