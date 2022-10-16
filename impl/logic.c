#include "data.h"
#include "io.h"
#include "utils.h"
#include "macros.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int comp(const void* info1_void, const void* info2_void) {
    const struct Software* info1 = (const struct Software*)info1_void;
    const struct Software* info2 = (const struct Software*)info2_void;
    const int class_diff = info1->func_class - info2->func_class;
    if (class_diff != 0) {
        return class_diff;
    }
    return strcmp(info1->name, info2->name);
}

void get_cwd(char* cwd) {
    char* ret = getcwd(cwd, 254);
    EXIT_IF(ret == NULL, "Could not obtain cwd");
    int cwd_len = strlen(cwd);
    cwd[cwd_len] = '/';
    cwd[cwd_len + 1] = '\0';
}

void convert_path_to_absolute(char* absolute_path, const char* cwd, const char* path) {
    strcpy(absolute_path, cwd);
    strcat(absolute_path, path);
}

void process_input_file(const char* input_file, const char* output_file) {
    EXIT_IF(input_file == NULL || output_file == NULL, "Got null pointer as input");

    char cwd[256];
    get_cwd(cwd);

    char absolute_input_path[256];
    char absolute_output_path[256];
    convert_path_to_absolute(absolute_input_path, cwd, input_file);
    convert_path_to_absolute(absolute_output_path, cwd, output_file);

    uint n = 0;
    FILE* file = fopen(absolute_input_path, "r");
    EXIT_IF(file == NULL, "Could not open file");

    struct Software* info = read_software_info(file, &n);
    fclose(file);
    EXIT_IF(info == NULL, "Could not read software information");

    uint n_filtered = n;
    struct Software* filtered = filter(info, &n_filtered, check);
    EXIT_IF_WITH_CALLBACK(filtered == NULL, "Could not filter software information", 
        free_software_array(info, n));
    
    qsort(filtered, n_filtered, sizeof(struct Software), comp);

    file = fopen(absolute_output_path, "w");
    EXIT_IF(file == NULL, "Could not open output file");

    fprintf(file, "%u\n", n_filtered);

    for (uint i = 0; i < n_filtered; ++i) {
        int ret = print_software_info(&filtered[i], file);
        fflush(file);
        EXIT_IF_WITH_CALLBACK(ret == -1, "Could not write software information",
            { free_software_array(info, n); free_software_array(filtered, n_filtered); fclose(file);});
    }

    free_software_array(info, n);
    free_software_array(filtered, n_filtered);
    fclose(file);
}
