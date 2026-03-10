#include "AVLTree.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>

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

void findRecord(AVLTree* tree, char* key)
{

}

void addRecord(AVLTree* tree, char* keyValue)
{

}

void deleteRecord(AVLTree* tree, char* key)
{

}

void saveRecords(FILE* file, AVLTree* tree)
{

}

void printHelp()
{
    printf("Commands:\n");
    printf("'find [key]' is used to print a record by key.\n");
    printf("'add [key:value]' is used to add a new record temporarily.\n");
    printf("'delete [key]' is used to delete a record by key temporarily.\n");
    printf("'save' is used to write temporarily data in the opened file.\n");
    printf("'quit' is used to quit\n");
}

/*
 * Return false if user wants to quit.
 * Return true otherwise.
 */
bool processCommand(AVLTree* tree, FILE* file, char* command, char* specifier)
{
    if (strcmp("find", command) == 0) {
        findRecord(tree, specifier);
    } else if (strcmp("add", command) == 0) {
        addRecord(tree, specifier);
    } else if (strcmp("delete", specifier) == 0) {
        deleteRecord(tree, specifier);
    } else if (strcmp("save", command) == 0) {
        saveRecords(file, tree);
    } else if (strcmp("quit", command) == 0) {
        return false;
    } else if(strcmp("help", command) == 0) {
        printHelp();
    } else {
        fprintf(stderr, "Undefined command. Try 'help'.\n");
    }
    return true;
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

    char buf[1024] = {};
    while (true) {
        printf("> ");
        if (fgets(buf, sizeof(buf), stdin) == NULL)
            break;
        
        buf[strcspn(buf, "\n")] = '\0';
        int idx = 0;
        while (isspace(buf[idx]))
            idx++;

        
        char command[8] = {};
        char specifier[256] = {}; 
        if (sscanf(buf + idx, "%7[^ \n] %[^ \n]\n", command, specifier) < 0)
            break;
        printf ("Command = %s, specifier = %s\n", command, specifier);
        if (!processCommand(tree, file, command, specifier))
            break;
    }

    fclose(file);
    avlFree(&tree);
    return 0;
}