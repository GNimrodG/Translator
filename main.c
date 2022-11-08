#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
//#include "debugmalloc.h"

#define DEFAULT_TRANSLATION_FILE "./translate.txt"
#define EXIT_INVALID_PARAM 1
#define EXIT_INVALID_FILE 2

void printUsage() {
    printf("Usage: <-d|-e> [-i <translation file>] <input>");
}

bool containsOption(int argc, char **argv, const char *option) {
    for (int i = 1; i < argc; ++i) {
        if (!strcmp(option, argv[i]))
            return true;
    }

    return false;
}

char *getOptionValue(int argc, char **argv, const char *option) {
    for (int i = 1; i < (argc - 1); ++i) {
        if (!strcmp(option, argv[i]))
            return argv[i + 1];
    }

    return NULL;
}

/* djb2 Hash function - http://www.cse.yorku.ca/~oz/hash.html */
unsigned long hash(const unsigned char *str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

char *readLine(FILE *fp, bool *end) {
    char *line = (char *) malloc(sizeof(char));
    size_t len = 0;
    char c;

    while ((c = (char) fgetc(fp)) != '\n' && c != EOF) {
        line[len++] = c;
        line = (char *) realloc(line, (len + 1) * sizeof(char));
    }

    if (c == EOF && end != NULL) {
        *end = true;
    }

    if (line[len - 1] == '\r') {
        line[len - 1] = '\0';
        line = (char *) realloc(line, len * sizeof(char));
    } else {
        line[len] = '\0';
    }
    return line;
}

struct TreeNode {
    unsigned long key;
    char *value;
    struct TreeNode *left;
    struct TreeNode *right;
};

int main(int argc, char **argv) {
    bool encrypt = containsOption(argc, argv, "-e");
    bool decrypt = containsOption(argc, argv, "-d");
    char *inputFilePath = getOptionValue(argc, argv, "-i");
    if (inputFilePath == NULL) inputFilePath = DEFAULT_TRANSLATION_FILE;

    if (!(encrypt || decrypt) || (encrypt && decrypt)) {
        printUsage();
        return EXIT_INVALID_PARAM;
    }

    if (access(inputFilePath, R_OK) != 0) {
        fprintf(stderr, "Unable to read the file '%s'!", inputFilePath);
        return EXIT_INVALID_FILE;
    }

    FILE *translationFile = fopen(inputFilePath, "r");
    if (translationFile == NULL) {
        fprintf(stderr, "Unable to read the file '%s'!", inputFilePath);
        return EXIT_INVALID_FILE;
    }

    bool fileEnd = false;
    char *sourceSpacer = readLine(translationFile, &fileEnd);
    if (fileEnd) {
        fprintf(stderr, "Invalid translation file! Only source spacer found!");
        return EXIT_INVALID_FILE;
    }
    char *targetSpacer = readLine(translationFile, &fileEnd);
    if (fileEnd) {
        fprintf(stderr, "Invalid translation file! Only source and target spacer found!");
        return EXIT_INVALID_FILE;
    }

    struct TreeNode *root = NULL;
    size_t lineCount = 0;

    while (!fileEnd) {
        char *line = readLine(translationFile, &fileEnd);

        char *source = strtok(line, '\t');
        char *target = strtok(NULL, '\t');

        free(line);

        if (source == NULL || target == NULL) {
            fprintf(stderr, "Invalid translation file! Invalid line: '%s'", line);
            return EXIT_INVALID_FILE;
        }

        struct TreeNode *node = (struct TreeNode *) malloc(sizeof(struct TreeNode));
        node->key = hash((const unsigned char *) source);
        node->value = target;
        node->left = NULL;
        node->right = NULL;

        if (root == NULL) {
            // Ha nincs gyökér, akkor ez lesz a gyökér
            root = node;
        } else {
            // Végigmegyünk a fákon, amíg nem találunk egy olyan csúcsot, ahol a bal vagy a jobb gyermek NULL
            struct TreeNode *current = root;
            while (true) {
                if (node->key < current->key) {
                    if (current->left == NULL) {
                        current->left = node;
                        break;
                    } else {
                        current = current->left;
                    }
                } else {
                    if (current->right == NULL) {
                        current->right = node;
                        break;
                    } else {
                        current = current->right;
                    }
                }
            }
        }
        lineCount++;
    }

    fclose(translationFile);
    printf("Translation file loaded! %zu lines loaded.\n", lineCount);

    return 0;
}