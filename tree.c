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

#include <stdlib.h>
#include <stdbool.h>
#include "debugmalloc.h"
#include "TreeNode.h"

// Beilleszt egy új elemet a fába a megfelelő helyre.
void insertNode(struct TreeNode **root, struct TreeNode *node) {
    if ((*root) == NULL) {
        // Ha nincs gyökér, akkor ez lesz a gyökér
        *root = node;
    } else {
        // Végigmegyünk az ágakon, amíg nem találunk egy olyan csúcsot, ahol a bal vagy a jobb gyermek NULL
        struct TreeNode *current = (*root);
        while (true) {
            if (strcasecmp(node->key, current->key) < 0) {
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
}

// Megkeresi a fában a megadott kulcsú elemet, és visszaadja annak értékét. Ha nem létezik NULL-t ad vissza.
char *find(const struct TreeNode *root, const char *key) {
    if (root == NULL) {
        return NULL;
    } else {
        const struct TreeNode *current = root;
        while (true) {
            int diff = strcasecmp(key, current->key);
            if (diff == 0) {
                return current->value;
            } else if (diff < 0) {
                if (current->left == NULL) {
                    return NULL;
                } else {
                    current = current->left;
                }
            } else {
                if (current->right == NULL) {
                    return NULL;
                } else {
                    current = current->right;
                }
            }
        }
    }
}

// Felszabadítja a fában található összes elemet és a fát is.
void freeTree(struct TreeNode *node) {
    if (node == NULL)
        return;

    freeTree(node->left);
    freeTree(node->right);
    free(node->key);
    free(node->value);
    free(node);
}