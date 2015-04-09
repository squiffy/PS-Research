#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

struct Entry {

    char name[8];
    uint32_t offset;
    uint32_t size;

};

int main(int argc, char *argv[]) {

    if(argc < 3) {
        printf("Usage: %s [pck file] [output directory]\n", argv[0]);
        exit(-1);
    }

    FILE *fp = fopen(argv[1], "r");

    printf("%15s%15s%15s\n", "Name", "Offset", "Size");

    int i;
    for(i = 0; i < 255; i++) {

        struct Entry entry;
        fread(&entry, sizeof(entry), 1, fp);
        if(entry.size == 0)
            break;
        printf("%15.8s", entry.name); 
        printf("%#15x", entry.offset); 
        printf("%#15x\n", entry.size); 

        char data[entry.size];

        size_t filenameSize = strlen(argv[1]) + 9;

        char filename[filenameSize];
        filename[filenameSize] = '\0';

        sprintf(filename, "%s%.8s", argv[2], entry.name);

        mkdir(argv[2], S_IRWXU);

        long pos = ftell(fp);
        fseek(fp, entry.offset, SEEK_SET);
        fread(&data, sizeof(data), 1, fp);

        FILE *file = fopen(filename, "w");
        fwrite(data, sizeof(data), 1, file);
        fclose(file);

        fseek(fp, pos, SEEK_SET);
        
    }

}
