#include <stdio.h>
#include <stdlib.h>

typedef struct TreeNode {
    char Data;
    struct TreeNode* parent;
    struct TreeNode* son;
    struct TreeNode* brother;
} TreeNode;

int createNode(char data, TreeNode** res) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    if (newNode == NULL) {  
        return 1; 
    }
    newNode->Data = data;
    newNode->son = NULL;
    newNode->brother = NULL;
    *res = newNode;
    return 0;
}

int buildTree(char* expression, int* index, TreeNode** res) {
    TreeNode* root = NULL;

    while (expression[*index] != '\0') {
        if (expression[*index] == '(') {
            (*index)++;
            TreeNode* newNode;
            int ansCreate = createNode(expression[*index], &newNode);
            if (ansCreate != 0) { 
                return ansCreate;
            }
            (*index)++;

            int ansBuild = buildTree(expression, index, &newNode->son);
            if (ansBuild != 0) { 
                return ansBuild;
            }

            if (root == NULL) {
                root = newNode;
            } else {
                TreeNode* temp = root;
                while (temp->son != NULL) {
                    temp = temp->son;
                }
                temp->son = newNode;
            }
            (*index)++;

        } else if (expression[*index] == ')') {
            (*index)++;
            *res = root;
            return 0;
        } else if (expression[*index] == ',') {
            (*index)++;
            TreeNode* newNode;
            int ansCreate = createNode(expression[*index], &newNode);
            if (ansCreate != 0) { 
                return ansCreate;
            }
            (*index)++;

            int ansBuild = buildTree(expression, index, &newNode->brother);
            if (ansBuild != 0) { 
                return ansBuild;
            }

            if (root == NULL) {
                root = newNode;
            } else {
                TreeNode* temp = root;
                while (temp->brother != NULL) {
                    temp = temp->brother;
                }
                temp->brother = newNode;
            }
            (*index)++;

        } else {
            TreeNode* newNode;
            int ansCreate = createNode(expression[*index], &newNode);
            if (ansCreate != 0) { 
                return ansCreate;
            }
            (*index)++;
            if (root == NULL) {
                root = newNode;
            } else {
                TreeNode* temp = root;
                while (temp->brother != NULL) {
                    temp = temp->brother;
                }
                temp->brother = newNode;
            }
        }
    }

    *res = root;
    return 0;
}

void printTree(struct TreeNode* root, int level) {
    if (root != NULL) {
        for (int i = 0; i < level; i++) {
            printf(" ");
        }
        printf( "%c\n", root->Data);
        if (root->son != NULL || root->brother != NULL) {
            printTree(root->son, level + 1);
            printTree(root->brother, level);
        }
    }
}

int main() {
    // if (argc != 3) {
    //     printf("Usage: %s input_file output_file\n", argv[0]);
    //     return 1;
    // }

    // FILE* inputFile = fopen(argv[1], "r");
    // if (inputFile == NULL) {
    //     perror("Error opening input file");
    //     return 1;
    // }

    // FILE* outputFile = fopen(argv[2], "w");
    // if (outputFile == NULL) {
    //     perror("Error opening output file");
    //     fclose(inputFile);
    //     return 1;
    // }

    char expression[] = "A(B(E(G,T,R(W,Z)),F(L,M)),C)";  // Adjust the size as needed

    
        int index = 0;
        struct TreeNode* root;
        int ansBuild = buildTree(expression, &index, &root);
        if (ansBuild != 0) {
            printf( "Error building tree for expression: %s\n", expression);
        } else {
            printTree(root, 0);
            printf("\n");
        }
    

    // fclose(inputFile);
    // fclose(outputFile);

    return 0;
}
