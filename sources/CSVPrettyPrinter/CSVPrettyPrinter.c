#include "CSVPrettyPrinter.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 Dynamically allocated fields, fieldWidths.
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
    LineNode* lastNode;
    int fieldsCount;
    int* fieldMaxWidths;
};

static int imax(int a, int b)
{
    return a > b ? a : b;
}

/* Reads line and deletes newline character */
static char* myReadLine(char* s, int n, FILE* stream)
{
    char* p = fgets(s, n, stream);
    if (p == NULL)
        return NULL;
    int size = strlen(p);
    if (size > 0 && p[size - 1] == '\n')
        p[size - 1] = '\0';

    return p;
}

/*
 Free *node pointer and sets *node=NULL.
 Checks if *node fields are not NULL.
 */
static void freeLineNode(LineNode** node, int fieldsCount)
{
    if ((*node) == NULL)
        return;
    if ((*node)->fields != NULL) {
        for (int i = 0; i < fieldsCount; i++)
            if ((*node)->fields[i] != NULL)
                free((*node)->fields[i]);
        free((*node)->fields);
    }
    if ((*node)->fieldWidths != NULL)
        free((*node)->fieldWidths);
    free(*node);
    *node = NULL;
}

/*
 s - the line containing fields,
 size - where size of the field will be stored,
 i - starting index of the field.
 Returns valid string or NULL if field is empty or error occurred.
 *size is always >= 0, *size = -1 if error occured.
 */
static char* copyField(const char* s, int* size, int* i)
{
    if (s == NULL || size == NULL) {
        *size = -1;
        return NULL;
    }

    while (isspace(s[*i]))
        (*i)++;

    if (s[*i] == '\0' || s[*i] == ',') {
        *size = 0;
        return NULL;
    }

    char buf[BUFSIZ] = {};
    bool hasQuote = s[*i] == '"';
    if (!hasQuote) {
        buf[0] = s[*i];
        *size = 1;
    } else {
        *size = 0;
    }

    (*i)++;
    for (; *i < BUFSIZ; (*i)++) {
        char c = s[*i];
        if (c == '\0' || (!hasQuote && c == ','))
            break;
        if (hasQuote && c == '\"') {
            if (s[++(*i)] != '\"') {
                break;
            }
        }
        buf[(*size)++] = c;
    }
    buf[(*size)] = '\0';
    return strdup(buf);
}

/*
 Count fields in a line.
 It is called one time when reading the first line
 to initialize CSVData struct.
 */
static int countFields(const char* s)
{
    if (s == NULL || s[0] == '\0')
        return 0;
    int res = 0;
    bool hasQuote = s[0] == '"';
    bool hasNullDelimiter = false;
    for (int i = 1; !hasNullDelimiter; i++) {
        switch (s[i]) {
        case ',':
            if (!hasQuote) {
                res++;
                if (s[++i] == '"') {
                    hasQuote = true;
                    i++;
                } else {
                    hasQuote = false;
                }
            }
            break;
        case '"':
            if (s[i + 1] != '"') {
                hasQuote = false;
            }
            break;
        case '\0':
            res++;
            hasNullDelimiter = true;
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
    if (data->fieldsCount <= 0)
        return false;
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
static bool parseLine(const char* line, CSVData* data)
{
    if (line == NULL)
        return false;

    if (data->head == NULL) {
        if (!parseInitLine(data, line)) {
            return false;
        }
    }
    /* Allocate all the data needed for the node. */
    LineNode* node = calloc(1, sizeof(*node));
    if (node == NULL)
        return false;

    node->fieldWidths = malloc(sizeof(node->fieldWidths[0]) * data->fieldsCount);
    if (node->fieldWidths == NULL) {
        free(node);
        return false;
    }
    node->fields = malloc(sizeof(char*) * data->fieldsCount);
    if (node->fields == NULL) {
        free(node->fieldWidths);
        free(node);
        return false;
    }
    node->next = NULL;

    /* Count widths of the fields and copy them. */
    int fieldIdx = 0;
    for (int i = 0; line[i] != '\0'; i++) {
        int curSize = 0;
        node->fields[fieldIdx] = copyField(line, &curSize, &i);
        if (curSize == -1) {
            freeLineNode(&node, data->fieldsCount);
            return false;
        }
        node->fieldWidths[fieldIdx] = curSize;
        data->fieldMaxWidths[fieldIdx] = imax(curSize, data->fieldMaxWidths[fieldIdx]);
        fieldIdx++;
    }

    if (data->head == NULL) {
        data->head = node;
        data->lastNode = node;
    } else {
        data->lastNode->next = node;
        data->lastNode = node;
    }
    return true;
}

CSVData* CSVDataRead(FILE* fp)
{
    CSVData* data = malloc(sizeof(*data));
    if (data == NULL)
        return NULL;

    data->head = NULL;
    data->fieldsCount = 0;
    data->fieldMaxWidths = NULL;
    data->lastNode = NULL;

    if (feof(fp))
        return data;

    char buf[BUFSIZ] = {};

    while (!feof(fp)) {
        char* line = myReadLine(buf, sizeof(buf), fp);
        if (line == NULL || line[0] == '\0')
            break;
        if (!parseLine(line, data)) {
            CSVDataFree(&data);
            return NULL;
        }
    }
    return data;
}

/* Draws line for table in buffer and returns it. */
static char* drawRow(char* buf, const int* fieldWidths, int fieldsCount, char rowChar)
{
    int i = 0;
    for (int f = 0; f < fieldsCount; f++) {
        buf[i++] = '+';
        memset(buf + i, rowChar, fieldWidths[f]);
        i += fieldWidths[f];
    }
    buf[i++] = '+';
    buf[i] = '\0';
    return buf;
}

static bool isNumber(const char* s, int size)
{
    if (size == 0)
        return false;
    int i = (s[0] == '+' || s[0] == '-') ? 1 : 0;
    for (; i < size && s[i] != '\0'; i++) {
        char c = s[i];
        if (('0' > c || c > '9') && c != '.')
            return false;
    }
    return true;
}

/* Draws fields of line in buffer and returns it. */
static char* drawFields(char* buf, const LineNode* node, int fieldsCount, const int* fieldMaxWidths, bool leftJustified)
{
    int i = 0;
    for (int f = 0; f < fieldsCount; f++) {
        bool isRightJustified = !leftJustified && isNumber(node->fields[f], node->fieldWidths[f]);
        buf[i++] = '|';
        /* Fields may be empty */
        if (node->fields[f] != NULL) {
            const char* fmt = isRightJustified ? "%*s" : "%-*s";
            sprintf(buf + i, fmt, fieldMaxWidths[f], node->fields[f]);
        } else {
            memset(buf + i, ' ', fieldMaxWidths[f]);
        }
        i += fieldMaxWidths[f];
    }
    buf[i++] = '|';
    buf[i] = '\0';
    return buf;
}

bool CSVDataWrite(const CSVData* data, FILE* fp)
{
    /* Empty table */
    if (data == NULL || data->head == NULL)
        return true;

    char buf[BUFSIZ] = {};
    if (fprintf(fp, "%s\n", drawRow(buf, data->fieldMaxWidths, data->fieldsCount, '=')) < 0)
        return false;
    if (fprintf(fp, "%s\n", drawFields(buf, data->head, data->fieldsCount, data->fieldMaxWidths, true)) < 0)
        return false;
    if (fprintf(fp, "%s\n", drawRow(buf, data->fieldMaxWidths, data->fieldsCount, '=')) < 0)
        return false;

    for (LineNode* p = data->head->next; p != NULL; p = p->next) {
        if (fprintf(fp, "%s\n", drawFields(buf, p, data->fieldsCount, data->fieldMaxWidths, false)) < 0)
            return false;
        if (fprintf(fp, "%s\n", drawRow(buf, data->fieldMaxWidths, data->fieldsCount, '-')) < 0)
            return false;
    }
    return true;
}

/*
 Free *data pointer and sets *data=NULL.
 Checks if *data fields are not NULL.
 */
void CSVDataFree(CSVData** data)
{
    if (*data == NULL)
        return;
    if ((*data)->fieldMaxWidths != NULL)
        free((*data)->fieldMaxWidths);
    LineNode* p = (*data)->head;
    while (p != NULL) {
        LineNode* prev = p;
        p = p->next;
        freeLineNode(&prev, (*data)->fieldsCount);
    }
    free(*data);
    *data = NULL;
}