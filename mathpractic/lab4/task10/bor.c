#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ALPHABET_SIZE 37

typedef struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];
    bool isEndOfWord;
} TrieNode;

TrieNode *getNode(void) {
    TrieNode *node = (TrieNode *)malloc(sizeof(TrieNode));
    node->isEndOfWord = false;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        node->children[i] = NULL;
    }
    return node;
}

int charToIndex(char c) {
    if ('a' <= c && c <= 'z') return c - 'a';
    if ('0' <= c && c <= '9') return 26 + c - '0';
    if (c == '_') return 36;
    return -1; 
}

void insert(TrieNode *root, const char *key) {
    TrieNode *node = root;
    for (int i = 0; i < strlen(key); i++) {
        int index = charToIndex(key[i]);
        if (index == -1) {
            printf("noдопустимый символ: %c\n", key[i]);
            return;
        }
        if (!node->children[index]) {
            node->children[index] = getNode();
        }
        node = node->children[index];
    }
    node->isEndOfWord = true;
}

bool search(TrieNode *root, const char *key) {
    TrieNode *node = root;
    for (int i = 0; i < strlen(key); i++) {
        int index = charToIndex(key[i]);
        if (index == -1) return false;
        if (!node->children[index]) return false;
        node = node->children[index];
    }
    return (node != NULL && node->isEndOfWord);
}

bool isLeafNode(TrieNode *node) {
    return node->isEndOfWord != false;
}

bool deleteHelper(TrieNode *node, const char *key, int depth) {
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

// Удаление слова из Trie
void delete(struct TrieNode *root, const char *key) {
deleteHelper(root, key, 0);
}

// int main() {
//     char keys[][8] = {"the", "a", "there", "answer", "any", "by", "bye", "their"};
//     int n = sizeof(keys) / sizeof(keys[0]);

//     struct TrieNode *root = getNode();

//     for (int i = 0; i < n; i++) {
//         insert(root, keys[i]);
//     }

//     search(root, "the") ? printf("the --- find\n") : printf("the --- no find\n");
//     search(root, "these") ? printf("these --- find\n") : printf("these --- no find\n");
//     search(root, "their") ? printf("their --- find\n") : printf("their --- no find\n");
//     search(root, "thaw") ? printf("thaw --- find\n") : printf("thaw --- no find\n");

//     delete(root, "the");
//     search(root, "the") ? printf("the --- find\n") : printf("the --- no find\n");

//     return 0;
// }
