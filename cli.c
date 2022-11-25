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
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "debugmalloc.h"

// Prints the correct usage of the program.
void printUsage() {
    printf("Usage: <-d|-e> [-i <translation file>] [-x <invalid value replacement>] <input>");
}

// Return true if the option is present in the arguments.
bool containsOption(int argc, char **argv, const char *option) {
    for (int i = 1; i < argc; ++i) {
        if (!strcmp(option, argv[i]))
            return true;
    }

    return false;
}

// Return the value of the option, or NULL if the option is not present.
char *getOptionValue(int argc, char **argv, const char *option) {
    for (int i = 1; i < (argc - 1); ++i) {
        if (!strcmp(option, argv[i]))
            return argv[i + 1];
    }

    return NULL;
}

// Returns the rest of the parameters after the start index as a string.
char *getRestParams(int argc, char **argv, int start) {
    char *rest = (char *) malloc(sizeof(char));
    size_t len = 1;
    rest[0] = '\0';

    for (int i = start; i < argc; ++i) {
        bool notLast = i < (argc - 1);
        len += strlen(argv[i]) + notLast;
        rest = (char *) realloc(rest, len * sizeof(char));
        strcat(rest, argv[i]);
        if (notLast)
            strcat(rest, " ");
    }

    return rest;
}