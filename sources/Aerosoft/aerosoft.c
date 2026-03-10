#include "AVLTree.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define STRINGINIZE(val) STRINGINIZE_DUMMY(val)
#define STRINGINIZE_DUMMY(val) #val

// For buffer overflow in loadData()
#define KEY_SIZE 31
#define VALUE_SIZE 255
/*
 * For AVL-tree comparator
 */
int myStrcmp(void* s1, void* s2)
{
    return strcmp(s1, s2);
}

/*
 * Read database and load data into AVL-tree.
 * Return NULL if error ocurred.
 */
AVLTree* loadData(FILE* file)
{
    AVLTree* tree = avlAlloc(myStrcmp, free, free);
    if (tree == NULL) {
        fprintf(stderr, "Failed to allocate memory!\n");
        return NULL;
    }
    
    char buf[BUFSIZ] = {};
    char key[KEY_SIZE+1] = {};
    char value[VALUE_SIZE+1] = {};
    while (!feof(file)) {
        if (fscanf(file,
             "%"STRINGINIZE(KEY_SIZE)"[^:]:%"STRINGINIZE(VALUE_SIZE)"[^\n]\n",
             key, value) != 2) {
                break;
        }

        char* newKey = strdup(key);
        if (newKey == NULL) {
            fprintf(stderr, "Failed to allocate memory!\n");
            avlFree(&tree);
            return NULL;
        }

        char* newValue = strdup(value);
        if (newValue == NULL) {
            fprintf(stderr, "Failed to allocate memory!\n");
            free(newKey);
            avlFree(&tree);
            return NULL;
        }

        if (!avlInsert(tree, newKey, newValue)) {
            fprintf(stderr, "Failed to load data!\n");
            avlFree(&tree);
            free(newKey);
            free(newValue);
            return NULL;
        }
    }

    if (ferror(file)) {
        perror("Failed to read");
        avlFree(&tree); // Sets to NULL
    }

    return tree;
}

int main(int argc, char** argv)
{
    if (argc != 2) {
        fprintf(stderr, "Expected file to open. Usage: %s [filename]\n", argv[0]);
        return 1;
    }

    FILE* file = fopen(argv[1], "rw");
    if (file == NULL) {
        fprintf(stderr, "Failed to open \"%s\": %s\n", argv[1], strerror(errno));
        return 1;
    }

    AVLTree* tree = loadData(file);
    if (tree == NULL) {
        fprintf(stderr, "Failed to load data from \"%s\".\n", argv[1]);
        return 1;
    }

    printf("%d records are loaded, ready to work.\n", avlSize(tree));
    fclose(file);
    avlFree(&tree);
    return 0;
}