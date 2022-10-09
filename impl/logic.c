#include "data.h"
#include "io.h"
#include "utils.h"
#include "macros.h"

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

void process_input_file(const char* input_file, const char* output_file) {
    EXIT_IF(input_file == NULL || output_file == NULL, "Got null pointer as input");

    uint n = 0;
    struct Software* info = read_software_info(input_file, &n);
    EXIT_IF(info == NULL, "Could not read software information");

    uint n_filtered = n;
    struct Software* filtered = filter(info, &n_filtered, check);
    EXIT_IF_WITH_CALLBACK(filtered == NULL, "Could not filter software information", 
        free_software_array(info, n));
    
    qsort(filtered, n_filtered, sizeof(struct Software), comp);

    FILE* file = fopen(output_file, "w");
    EXIT_IF(file == NULL, "Could not open output file");

    for (uint i = 0; i < n_filtered; ++i) {
        int ret = print_software_info(&filtered[i], file);
        EXIT_IF_WITH_CALLBACK(ret == -1, "Could not write software information",
            { free_software_array(info, n); free_software_array(filtered, n_filtered); });
    }

    free_software_array(info, n);
    free_software_array(filtered, n_filtered);
}
