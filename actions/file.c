#include "file.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

extern const char * file_read_contents(const char * file) {
    FILE* hFile = fopen(file, "rb");
    if ( !hFile ) {
        return NULL;
    }
    fseek(hFile, 0, SEEK_END);
    size_t filesize = ftell(hFile); 
    rewind(hFile);

    char * buffer = malloc(filesize+1);
    if ( buffer == NULL ) {
        return NULL;
    } 
    size_t check = fread(buffer, 1, filesize, hFile);

    if ( check != filesize ) {
        free(buffer);
        return NULL;
    }
    buffer[filesize] = '\0';
    fclose(hFile);
    return buffer;
}

