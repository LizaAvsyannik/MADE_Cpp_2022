#include "utils.h"
#include "data.h"

#include <stdlib.h>

#define DAYS_IN_YEAR 365
#define DAYS_IN_HALF_YEAR DAYS_IN_YEAR / 2  
#define EPS 1e-3
#define SECONDS_IN_DAY 86400

struct Software* filter(const struct Software* info, uint* n, bool (*pred)(const struct Software*)) {
    int* indices = (int*)malloc(*n * sizeof(int));
    for (int i = 0; i < *n; ++i) {
        indices[i] = -1;
    }

    int index_ptr = 0;
    for (int i = 0; i < *n; ++i) {
        if (pred(&info[i])) {
            indices[index_ptr++] = i;
        }
    }

    struct Software* filtered = (struct Software*)malloc(index_ptr * sizeof(struct Software));
    for (int i = 0; i < index_ptr; ++i) {
        filtered[i] = copy_software_info(&info[indices[i]]);
    }
    *n = index_ptr;
    return filtered;
}

bool check(const struct Software* info) {
    return check_updates(&info->last_update_date, &info->installation_date) && 
            check_installation_date(&info->installation_date, DAYS_IN_HALF_YEAR);
}

bool check_updates(const struct Date* update_date, const struct Date* install_date) {
    return abs(compare_dates(update_date, install_date)) < EPS;
}

bool check_installation_date(const struct Date* date, uint days) {
    time_t time_now;
    time(&time_now);

    time_t time_half_year_ago = time_now - (DAYS_IN_HALF_YEAR * SECONDS_IN_DAY);
    time_t days_since_epoch = convert_to_time(date);

    double diff = difftime(days_since_epoch, time_half_year_ago);
    return diff >= 0;
}
