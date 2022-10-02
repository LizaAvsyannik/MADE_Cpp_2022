#include "data.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void free_software(struct Software* item) {
    free(item->name);
}

void free_software_array(struct Software* arr, uint n) {
    for (uint i = 0; i < n; ++i) {
        free_software(&arr[i]);
    }
    free(arr);
}

struct Software copy_software_info(const struct Software* info){
    struct Software copy = *info;
    copy.name = (char*)malloc((MAX_NAME_LENGTH + 1) * sizeof(char));
    strcpy(copy.name, info->name);
    return copy;
}

time_t convert_to_time(const struct Date* date) {
    struct tm st = {.tm_mday = date->day,
                    .tm_mon = date->month - 1, // 0-based so 6 is July
                    .tm_year = date->year - 1900};
    time_t t = mktime(&st);
    return t;
}

int compare_dates(const struct Date* date1, const struct Date* date2) {
    time_t days1_since_epoch = convert_to_time(date1);
    time_t days2_since_epoch = convert_to_time(date2);
    double diff = difftime(days1_since_epoch, days2_since_epoch);
    return diff;
}
