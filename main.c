// Translator (aka MorzeNHF) - Universal translator program with CLI
// Copyright (c) 2022 Nimród Glöckl
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <io.h>
#include "debugmalloc.h"
#include "cli.h"
#include "TreeNode.h"
#include "tree.h"
#include "file.h"

const char *DEFAULT_TRANSLATION_FILE = "translate.txt";
const int EXIT_INVALID_PARAM = 1;
const int EXIT_INVALID_FILE = 2;
const char *TRANSLATION_DELIMETER = "\t";

// Source: https://stackoverflow.com/questions/58244300/getting-the-error-undefined-reference-to-strsep-with-clang-and-mingw
char *strsep(char **stringp, const char *delim) {
    char *rv = *stringp;
    if (rv) {
        *stringp += strcspn(*stringp, delim);
        if (**stringp)
            *(*stringp)++ = '\0';
        else
            *stringp = 0;
    }
    return rv;
}

int main(int argc, char **argv) {
    const bool encode = containsOption(argc, argv, "-e");
    const bool decode = containsOption(argc, argv, "-d");
    const bool verbose = containsOption(argc, argv, "-v");
    const char *inputFileParam = getOptionValue(argc, argv, "-i");
    const char *invalidValue = getOptionValue(argc, argv, "-x");
    const char *inputFile = inputFileParam == NULL ? DEFAULT_TRANSLATION_FILE : inputFileParam;

    if (!(encode || decode) || (encode && decode)) {
        printUsage();
        if (inputFileParam != NULL)
            free((void *) inputFileParam);
        return EXIT_INVALID_PARAM;
    }

    // filename + (-d or -e) = 2 params by default
    char *restParams = getRestParams(argc, argv, 2 + (inputFileParam != NULL ? 2 : 0) + verbose + (invalidValue != NULL ? 2 : 0));

    if (access(inputFile, R_OK) != 0) {
        fprintf(stderr, "Unable to read the file '%s'!\n", inputFile);

        free(restParams);

        return EXIT_INVALID_FILE;
    }

    FILE *translationFile = fopen(inputFile, "r");
    if (translationFile == NULL) {
        fprintf(stderr, "Unable to read the file '%s'!\n", inputFile);

        free(restParams);

        return EXIT_INVALID_FILE;
    }

    bool fileEnd = false;
    char *leftSpacer = readLine(translationFile, &fileEnd);
    if (fileEnd) {
        fprintf(stderr, "Invalid translation file! Only source left side spacer character found!\n");

        fclose(translationFile);
        free(leftSpacer);
        free(restParams);

        return EXIT_INVALID_FILE;
    }
    char *rightSpacer = readLine(translationFile, &fileEnd);
    if (fileEnd) {
        fprintf(stderr, "Invalid translation file! Only left and right spacer character found!\n");

        fclose(translationFile);
        free(leftSpacer);
        free(rightSpacer);
        free(restParams);

        return EXIT_INVALID_FILE;
    }

    if (verbose) {
        printf("Left spacer: '%s'\tRight spacer: '%s'\n", leftSpacer, rightSpacer);
    }

    struct TreeNode *root = NULL;
    size_t lineCount = 0;

    while (!fileEnd) {
        char *line = readLine(translationFile, &fileEnd);
        if (line == NULL || strlen(line) == 0 || line[0] == '#') {
            free(line);
            continue;
        }

        char *source = strsep(&line, TRANSLATION_DELIMETER);
        char *target = strsep(&line, TRANSLATION_DELIMETER);

        if (source == NULL || target == NULL) {
            fprintf(stderr, "Invalid translation file! Invalid line: '%s'\n", line);
            free(leftSpacer);
            free(rightSpacer);
            free(source);
            free(target);
            free(line);
            free(restParams);
            freeTree(root);

            return EXIT_INVALID_FILE;
        }

        struct TreeNode *node = (struct TreeNode *) malloc(sizeof(struct TreeNode));
        node->key = (char *) malloc(strlen(encode ? source : target) + 1 * sizeof(char));
        strcpy(node->key, (encode ? source : target));

        node->value = (char *) malloc((strlen(encode ? target : source) + 1) * sizeof(char));
        strcpy(node->value, (encode ? target : source));

        free(source);
        free(line);
        node->left = NULL;
        node->right = NULL;

        insertNode(&root, node);

        lineCount++;
    }

    fclose(translationFile);

    if (verbose) {
        printf("Loaded %zu translations from file '%s'.\n", lineCount, inputFile);
    }

    char *sourceSpacer = encode ? leftSpacer : rightSpacer;
    char *targetSpacer = encode ? rightSpacer : leftSpacer;
    char *originalRestParams = restParams;

    if (strcmp(sourceSpacer, "") == 0) {
        for (int i = 0; i < strlen(restParams); i++) {
            char *source = (char *) malloc(2 * sizeof(char));
            source[0] = restParams[i];
            source[1] = '\0';

            char *found = find(root, source);
            printf("%s%s", (found == NULL) ? source : found, targetSpacer);
            free(source);
        }
    } else {
        char *part;
        while ((part = strsep(&restParams, sourceSpacer)) != NULL) {
            if (strlen(part) == 0) {
                continue;
            }

            char *translated = find(root, part);
            printf("%s%s", translated == NULL ? (invalidValue != NULL ? invalidValue : part) : translated, targetSpacer);
        }
        free(part);
    }

    printf("\n");

    freeTree(root);
    free(leftSpacer);
    free(rightSpacer);
    free(originalRestParams);

    return 0;
}
