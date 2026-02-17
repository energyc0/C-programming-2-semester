#include "CSVPrettyPrinter.h"

#include <stdlib.h>

struct CSVData {
};

CSVData* CSVDataRead(FILE* fp)
{
    CSVData* data = malloc(sizeof(*data));
    if (data == NULL)
        return NULL;

    return data;
}

int CSVDataWrite(const CSVData* data, FILE* fp)
{
    return 1;
}