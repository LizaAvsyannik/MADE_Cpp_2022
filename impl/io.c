#include "io.h"
#include "data.h"
#include "macros.h"

#include <stdlib.h>
#include <string.h>

struct Software* read_software_info(const char* filename, uint* n) {
    VERIFY_OR(filename != NULL && n != NULL, NULL, "Got null pointer as input");

    FILE* file = fopen(filename, "r");
    VERIFY_OR(file != NULL, NULL, "Could not open file");

    int ret = fscanf(file, "%u\n", n);
    VERIFY_OR(ret != EOF, NULL, "EOF during read");

    struct Software* info = (struct Software*)malloc(*n * sizeof(struct Software));
    VERIFY_OR(info != NULL, NULL, "Could not allocate memory with malloc");
    for (uint i = 0; i < *n; ++i) {
        init_software_info(&info[i]);
        int ret = read_single_software_info(&info[i], file);
        VERIFY_OR_WITH_CALLBACK(ret != -1, NULL, "Could not read from file",
            free_software_array(info, *n));
    }
    fclose(file);
    return info;
}

int read_single_software_info(struct Software* item, FILE* file) {
    VERIFY_OR(item != NULL && file != NULL, -1, "Got null pointer as input");

    item->name = (char*)malloc((MAX_NAME_LENGTH + 1) * sizeof(char));
    VERIFY_OR(item->name != NULL, -1, "Could not allocate memory with malloc");

    char* str_or_null = fgets(item->name, MAX_NAME_LENGTH, file);
    VERIFY_OR_WITH_CALLBACK(str_or_null != NULL, -1, "Could not read from file",
        free(item->name));

    int str_length = strlen(item->name);
    if (item->name[str_length - 1] == '\n') {
        item->name[str_length - 1] = '\0';
    }

    uint func_class = 0;
    int ret = fscanf(file, "%d %u %u %u %u\n", &func_class, &item->version.major,
                     &item->version.minor, &item->version.patch, &item->version.build);
    item->func_class = (enum FunctionalClass)func_class;
    VERIFY_OR_WITH_CALLBACK(ret != EOF, -1, "EOF during read", 
        free(item->name));

    ret = fscanf(file, "%u %u %u\n", &item->installation_date.day,
                 &item->installation_date.month, &item->installation_date.year);
    VERIFY_OR_WITH_CALLBACK(ret != EOF, -1, "EOF during read", 
        free(item->name));
    
    ret = fscanf(file, "%u %u %u\n", &item->last_update_date.day,
                 &item->last_update_date.month, &item->last_update_date.year);
    VERIFY_OR_WITH_CALLBACK(ret != EOF, -1, "EOF during read", 
        free(item->name));
    return 0;
}

int print_software_info(const struct Software* info, FILE* file) {
    VERIFY_OR(info != NULL && file != NULL, -1, "Got null pointer as input");

    int ret = fprintf(file, "%s\n%d %u %u %u %u\n", info->name, info->func_class,
        info->version.major, info->version.minor, info->version.patch, info->version.build);
    VERIFY_OR(ret >= 0, -1, "Could not write to file");

    ret = fprintf(file, "%02u %02u %02u\n", info->installation_date.day,
        info->installation_date.month, info->installation_date.year);
    VERIFY_OR(ret >= 0, -1, "Could not write to file");

    ret = fprintf(file, "%02u %02u %02u\n", info->last_update_date.day,
        info->last_update_date.month, info->last_update_date.year);
    VERIFY_OR(ret >= 0, -1, "Could not write to file");

    return 0;
}
