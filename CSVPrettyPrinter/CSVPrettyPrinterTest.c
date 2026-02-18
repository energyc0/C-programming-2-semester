#include "CSVPrettyPrinter.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
int main(int argc, char** argv)
{
    FILE* fp = fopen("csv_test.csv", "r");
    if (fp == NULL) {
        fprintf(stderr, "Failed to open \"%s\": %s\n", "csv_test.csv", strerror(errno));
        return 1;
    }
    CSVData* data = CSVDataRead(fp);
    if (!CSVDataWrite(data, stdout))
        fprintf(stderr, "Failed to write!\n");
    CSVDataFree(&data);
    return 0;
}