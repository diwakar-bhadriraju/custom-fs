#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 512

typedef struct {
    char filename[8];
    char extension[3];
    unsigned short start_block;
    unsigned int size;
} DirectoryEntry;

void read_directory(unsigned char *disk, DirectoryEntry *entries) {
    for (int i = 0; i < 512 / sizeof(DirectoryEntry); i++) {
        memcpy(entries + i, disk + BLOCK_SIZE + i * sizeof(DirectoryEntry), sizeof(DirectoryEntry));
    }
}

int main() {
    unsigned char *disk;
    DirectoryEntry entries[32];

    // Load the disk image into memory
    FILE *file = fopen("walkerOS.img", "rb");
    if (file == NULL) {
        fprintf(stderr, "Error opening file\n");
        return 1;
    }
    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);
    disk = malloc(size);
    if (fread(disk, 1, size, file) != size) {
        fprintf(stderr, "Error reading file\n");
        fclose(file);
        return 1;
    }
    fclose(file);

    // Read the directory entries from the disk
    read_directory(disk, entries);

    // Print the directory entries to the console
    printf("Directory Entries:\n");
    for (int i = 0; i < 32; i++) {
        if (entries[i].filename[0] == '\0') {
            break;
        }
        printf("%s.%s\tBlock: %u\tSize: %u bytes\n",
               entries[i].filename,
               entries[i].extension,
               entries[i].start_block,
               entries[i].size);
    }

    // Free the disk image from memory
    free(disk);

    return 0;
}