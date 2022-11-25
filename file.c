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
#include "debugmalloc.h"

// Újsorig vagy a fájl végéig olvas egy sort a fájlból, és visszaadja azt egy dinamikusan lefoglalt string(char*)-ben.
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