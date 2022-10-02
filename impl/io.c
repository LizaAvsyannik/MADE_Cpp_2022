#include "io.h"
#include "data.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define HANDLE_IO_ERROR(error_msg, abort) { handle_io_error(error_msg, __FILE__, __LINE__, abort); }
void handle_io_error(const char* error_msg, const char* file, int line, bool abort) {
    fprintf(stderr, "IO error: %s, in file %s, line %d\n", error_msg, file, line);
    if (abort) {
        exit(1);
    }
}

struct Software* read_software_info(const char* filename, uint* n) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        HANDLE_IO_ERROR("Could not open file", true);
    }
    int ret = fscanf(file, "%u\n", n);
    if (ret == EOF) {
        HANDLE_IO_ERROR("EOF during read", true);
    }
    struct Software* info = (struct Software*)malloc(*n * sizeof(struct Software));
    for (uint i = 0; i < *n; ++i) {
        read_single_software_info(&info[i], file);
    }
    fclose(file);
    return info;
}

void read_single_software_info(struct Software* item, FILE* file) {
    item->name = (char*)malloc((MAX_NAME_LENGTH + 1) * sizeof(char));
    char* str_or_null = fgets(item->name, MAX_NAME_LENGTH + 1, file);

    if (str_or_null == NULL) {
        HANDLE_IO_ERROR("Could not open file", true);
    }
    int str_length = strlen(item->name);
    if (item->name[str_length - 1] == '\n') {
        item->name[str_length - 1] = '\0';
    }
    uint func_class = 0;
    int ret = fscanf(file, "%d %u %u %u %u\n", &func_class, &item->version.major,
                     &item->version.minor, &item->version.patch, &item->version.build);
    item->func_class = (enum FunctionalClass)func_class;
    if (ret == EOF) {
        HANDLE_IO_ERROR("EOF during read", true);
    }
    ret = fscanf(file, "%u %u %u\n", &item->installation_date.day,
                 &item->installation_date.month, &item->installation_date.year);
    if (ret == EOF) {
        HANDLE_IO_ERROR("EOF during read", true);
    }
    ret = fscanf(file, "%u %u %u\n", &item->last_update_date.day,
                 &item->last_update_date.month, &item->last_update_date.year);
    if (ret == EOF) {
        HANDLE_IO_ERROR("EOF during read", true);
    }
}

void print_software_info(const struct Software* info, FILE* file) {
    if (info == NULL) {
        HANDLE_IO_ERROR("Got NULL as data to print", true);
    }
    if (file == NULL) {
        HANDLE_IO_ERROR("Invalid file descriptor", true);
    }
    fprintf(file, "%s\n%d %u %u %u %u\n", info->name, info->func_class,
            info->version.major, info->version.minor, info->version.patch, info->version.build);
    fprintf(file, "%02u %02u %02u\n", info->installation_date.day,
            info->installation_date.month, info->installation_date.year);
    fprintf(file, "%02u %02u %02u\n", info->last_update_date.day,
            info->last_update_date.month, info->last_update_date.year);
}
