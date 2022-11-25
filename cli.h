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

#ifndef MORZENHF_CLI_H
#define MORZENHF_CLI_H

#include <stdbool.h>

void printUsage();

bool containsOption(int argc, char **argv, const char *option);

char *getOptionValue(int argc, char **argv, const char *option);

char *getRestParams(int argc, char **argv, int start);

#endif //MORZENHF_CLI_H
