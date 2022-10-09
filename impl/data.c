#include "data.h"
#include "macros.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

void free_software(struct Software* item) {
    if (item != NULL) {
        free(item->name);
    }
}

void free_software_array(struct Software* arr, uint n) {
    if (arr != NULL) {
        for (uint i = 0; i < n; ++i) {
            free_software(&arr[i]);
        }
        free(arr);
    }
}

void init_software_info(struct Software* info) {
    info->name = NULL;
}

int copy_software_info(const struct Software* info, struct Software* new_info) {
    VERIFY_OR(info != NULL, -1, "Got null pointer as input");
    if (new_info->name != NULL) {
        free(new_info->name);
    }
    *new_info = *info;
    new_info->name = (char*)malloc((MAX_NAME_LENGTH + 1) * sizeof(char));
    VERIFY_OR(new_info->name != NULL, -1, "Could not allocate memory with malloc");
    strcpy(new_info->name, info->name);
    return 0;
}

time_t convert_to_time(const struct Date* date) {
    VERIFY_OR(date != NULL, 0, "Got null pointer as input");
    struct tm st = {.tm_mday = date->day,
                    .tm_mon = date->month - 1,
                    .tm_year = date->year - 1900};
    time_t t = mktime(&st);
    return t;
}

double compare_dates(const struct Date* date1, const struct Date* date2) {
    VERIFY_OR(date1 != NULL && date2 != NULL, NAN, "Got null pointer as input");
    time_t seconds1_since_epoch = convert_to_time(date1);
    time_t seconds2_since_epoch = convert_to_time(date2);
    double diff = difftime(seconds1_since_epoch, seconds2_since_epoch);
    return diff;
}
