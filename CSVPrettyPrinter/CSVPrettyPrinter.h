#pragma once

#include <stdio.h>

typedef struct CSVData CSVData;

/*
 Read CSV data from file descriptor to dynamically allocated struct.
 Return valid pointer on success, NULL otherwise.
 Write error details to errno.
 */
CSVData* CSVDataRead(FILE* fp);
/*
 Print CSV data to file descriptor.
 Return 1 on success, 0 otherwise.
 Write error details to errno.
 */
int CSVDataWrite(const CSVData* data, FILE* fp);

/*
 Free CSV data and set pointer to NULL.
 */
void CSVDataFree(CSVData** data);
