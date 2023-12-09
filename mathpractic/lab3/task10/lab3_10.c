#include "../header.h"

struct Node {
    char data;
    int childCount;
    struct Node** children;
};

struct Node* add_node(char data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode != NULL) {
        newNode->data = data;
        newNode->childCount = 0;
        newNode->children = NULL;
    }
    return newNode;
}

int add_child(struct Node* parent, struct Node* child) {
    parent->children = realloc(parent->children, (parent->childCount + 1) * sizeof(struct Node*));
    if (parent->children == NULL) {
        return NO_MEMORY;
    }
    parent->children[parent->childCount] = child;
    parent->childCount++;
    return OK;
}

void print_tree(struct Node* root, int level, FILE* fptr) {
    if (root != NULL) {
        for (int i = 0; i < level; i++) {
            fprintf(fptr, "   ");
        }
        if(level == 0) {
            fprintf(fptr, "[-%c\n", root->data);
        } else {
            fprintf(fptr, "\\-%c\n", root->data);
        }
        
        for (int i = 0; i < root->childCount; i++) {
            print_tree(root->children[i], level + 1, fptr);
        }
    }
}

void free_tree(struct Node* root) {
    if (root != NULL) {
        for (int i = 0; i < root->childCount; i++) {
            free_tree(root->children[i]);
        }
        free(root->children);
        free(root);
    }
}

int tree_by_brackets(struct Node** res, char* seq) {
    struct Node* root = add_node(seq[0]);
    if(root == NULL) {
        return NO_MEMORY;
    }
    struct Node* node;
    struct Node* curr = root;
    struct Node** parent_stack = (struct Node**)malloc(strlen(seq) * sizeof(struct Node*));
    if(parent_stack == NULL) {
        return NO_MEMORY;
    }

    int count_node = 0, length = strlen(seq);
    for (int i = 0; i < length; i++) {
        if (seq[i] == '(') {
            node = add_node(seq[i + 1]);
            parent_stack[count_node] = curr;
            count_node++;
            if (add_child(curr, node) != OK) {
                free(parent_stack);
                return NO_MEMORY;
            }
            curr = node;
        } else if (seq[i] == ',') {
            node = add_node(seq[i + 1]);
            if (add_child(parent_stack[count_node - 1], node) != OK) {
                free(parent_stack);
                return NO_MEMORY;
            }
            curr = node;
        } else if (seq[i] == ')') {
            parent_stack[count_node - 1] = NULL;
            count_node--;
        }
    }

    free(parent_stack);
    (*res) = root;
    return OK;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        print_err(INVALID_ARGC);
        return INVALID_ARGC;
    }

    FILE* inputFile = fopen(argv[1], "r");
    if (inputFile == NULL) {
        print_err(FILE_NOT_OPEN);
        return FILE_NOT_OPEN;
    }
    FILE* outputFile = fopen(argv[2], "w");
    if(outputFile == NULL) {
        fclose(inputFile);
        print_err(FILE_NOT_OPEN);
        return FILE_NOT_OPEN;
    }

    char buffer[BUFSIZ];
    while (fgets(buffer, sizeof(buffer), inputFile) != NULL) {
        //Удаление символа новой строки из конца буфера
        size_t length = strlen(buffer);
        if (length > 0 && buffer[length - 1] == '\n') {
            buffer[length - 1] = '\0';
        }

        struct Node* root;
        int st = tree_by_brackets(&root, buffer);
        if(st != OK) {
            fclose(inputFile);
            fclose(outputFile);
            free_tree(root);
            print_err(st);
            return st;
        }

        print_tree(root, 0, outputFile);
        fprintf(outputFile, "\n");
        free_tree(root);
    }

    fclose(inputFile);
    fclose(outputFile);

    return OK;
}
