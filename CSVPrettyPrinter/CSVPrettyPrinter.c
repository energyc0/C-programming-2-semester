#include "CSVPrettyPrinter.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 Dynamically allocated line, fieldWidths.
 */
typedef struct LineNode {
    char** fields;
    int* fieldWidths;
    struct LineNode* next;
} LineNode;

/*
 Linklist of LineNode's and dynamically allocated fieldMaxWidths.
 */
struct CSVData {
    LineNode* head;
    int fieldsCount;
    int* fieldMaxWidths;
};

static int imax(int a, int b)
{
    return a > b ? a : b;
}

/*
 Assign 'src' array max values between 'src' and 'dst' elements,
 'src' and 'dst arrays must be the 'size' size.
 */
static void setMaxArray(int* src, const int* dst, int size)
{
    for (int i = 0; i < size; i++)
        src[i] = imax(src[i], dst[i]);
}

/* Reads line and deletes newline character */
static char* myReadLine(char* s, int n, FILE* stream)
{
    char* p = fgets(s, n, stream);
    if(p == NULL)
        return NULL;
    int size = strlen(p);
    if (size > 0 && p[size - 1] == '\n')
        p[size-1] = '\0';

    //printf("Line read: (%s)\n", p);
    return p;
}

static void freeLineNode(LineNode** node)
{
    free((*node)->fields);
    free((*node)->fieldWidths);
    free(*node);
    *node = NULL;
}

/*
 s - the line containing fields,
 size - where size of the field will be stored,
 i - starting index of the field.
 */
static char* copyField(const char* s, int* size, int* i)
{
    if (s == NULL || size == NULL || s[0] == '\0')
        return NULL;
    char buf[BUFSIZ] = {};
    bool has_quote = s[*i] == '"';
    if (!has_quote) {
        buf[0] = s[*i];
        *size = 1;
    } else {
        *size = 0;
    }

    (*i)++;
    for (; *i < BUFSIZ; (*i)++) {
        char c = s[*i];
        if (c == '\0' || (!has_quote && c == ','))
            break;
        if (has_quote && c == '\"') {
            if (s[++(*i) + 1] != '\"') {
                break;
            }
        }
        buf[(*size)++] = c;
    }
    buf[(*size)] = '\0';
    return strdup(buf);
}

static int countFields(const char* s)
{
    if (s == NULL || s[0] == '\0')
        return 0;
    int res = 0;
    bool has_quote = s[0] == '"';
    bool has_null_delimiter = false;
    for (int i = 1; !has_null_delimiter; i++) {
        switch (s[i]) {
        case ',':
            if (!has_quote) {
                res++;
                if (s[++i] == '"') {
                    has_quote = true;
                    i++;
                } else {
                    has_quote = false;
                }
            }
            break;
        case '"':
            if (s[i + 1] != '"') {
                has_quote = false;
            }
            break;
        case '\0':
            res++;
            has_null_delimiter = true;
            break;
        }
    }
    return res;
}

/* Initializes 'fieldsCount' and 'fieldMaxWidths' of CSVData struct */
static bool parseInitLine(CSVData* data, const char* line)
{
    /* Count fields */
    data->fieldsCount = countFields(line);
    printf("%d\n", data->fieldsCount);
    const unsigned size = sizeof(data->fieldMaxWidths[0]) * data->fieldsCount;
    data->fieldMaxWidths = malloc(size);
    if (data->fieldMaxWidths == NULL)
        return false;
    memset(data->fieldMaxWidths, 0, size);
    return true;
}

/*
 Allocate LineNode struct,
 return NULL on fail, checks line == NULL.
 */
static LineNode* parseLine(char* line, int fieldsCount)
{
    if (line == NULL)
        return NULL;

    /* Allocate all the data needed for the node. */
    LineNode* node = malloc(sizeof(*node));
    if (node == NULL)
        return NULL;

    node->fieldWidths = malloc(sizeof(node->fieldWidths[0]) * fieldsCount);
    if (node->fieldWidths == NULL) {
        free(node);
        return NULL;
    }
    node->fields = malloc(sizeof(char*) * fieldsCount);
    if (node->fields == NULL) {
        free(node->fieldWidths);
        free(node);
        return NULL;
    }
    node->next = NULL;

    /* Count widths of the fields and copy them. */
    int fieldIdx = 0;
    for (int i = 0; line[i] != '\0'; i++) {
        int curSize = 0;
        node->fields[fieldIdx] = copyField(line, &curSize, &i);
        printf("DEBUG: (%.*s)\n",curSize, node->fields[fieldIdx]);
        if (node->fields[fieldIdx] == NULL) {
            freeLineNode(&node);
            return NULL;
        }
        node->fieldWidths[fieldIdx] = curSize;
        fieldIdx++;
    }

    return node;
}

CSVData* CSVDataRead(FILE* fp)
{
    CSVData* data = malloc(sizeof(*data));
    if (data == NULL)
        return NULL;

    data->head = NULL;
    data->fieldsCount = 0;
    data->fieldMaxWidths = NULL;

    if (feof(fp))
        return data;

    char buf[BUFSIZ] = {};
    char* line = myReadLine(buf, sizeof(buf), fp);
    if (line == NULL || !parseInitLine(data, line)) {
        free(data);
        return NULL;
    }

    printf("%lu\n", sizeof(CSVData));
    data->head = parseLine(line, data->fieldsCount);
    printf("%lu\n", sizeof(CSVData));
    if (data->head == NULL) {
        free(data->fieldMaxWidths);
        free(data);
        return NULL;
    }

    /* Node to insert. */
    LineNode* lastNode = data->head;
    printf("%lu\n", sizeof(CSVData));
    while (!feof(fp)) {
        line = myReadLine(buf, sizeof(buf), fp);
        printf("%lu\n", sizeof(CSVData));
        /* Checks if line == NULL and allocates new node. */
        lastNode->next = parseLine(line, data->fieldsCount);
        if (lastNode->next == NULL) {
            CSVDataFree(&data);
            return NULL;
        }
        lastNode = lastNode->next;
    }
    return data;
}

int CSVDataWrite(const CSVData* data, FILE* fp)
{
    for (LineNode* p = data->head; p != NULL; p = p->next) {
        for (int i = 0; i < data->fieldsCount; i++) {
            fprintf(fp, "%s;", p->fields[i]);
        }
        fputc('\n', fp);
    }
    return 1;
}

void CSVDataFree(CSVData** data)
{
    free((*data)->fieldMaxWidths);
    LineNode* p = (*data)->head;
    while (p != NULL) {
        LineNode* prev = p;
        p = p->next;
        free(prev);
    }
    free(*data);
    *data = NULL;
}