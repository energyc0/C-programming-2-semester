#include "CSVPrettyPrinter.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
int main(int argc, char** argv)
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s [input *.csv filename] [output filename]\n", argv[0]);
        return 1;
    }
    FILE* fpin = fopen(argv[1], "r");
    if (fpin == NULL) {
        fprintf(stderr, "Failed to open \"%s\": %s\n", argv[1], strerror(errno));
        return 1;
    }
    CSVData* data = CSVDataRead(fpin);
    fclose(fpin);
    if (data == NULL) {
        fprintf(stderr, "Failed to parse \"%s\".\n", argv[1]);
        return 1;
    }

    FILE* fpout = fopen(argv[2], "w");
    if (fpout == NULL) {
        CSVDataFree(&data);
        fprintf(stderr, "Failed to open for writing \"%s\": %s\n", argv[2], strerror(errno));
        return 1;
    }
    if (!CSVDataWrite(data, fpout)) {
        fclose(fpout);
        CSVDataFree(&data);
        fprintf(stderr, "Failed to write to \"%s\": %s\n", argv[2], strerror(errno));
        return 1;
    }
    fclose(fpout);
    CSVDataFree(&data);
    return 0;
}