#include "utils.h"
#include "data.h"
#include "macros.h"

#include <stdlib.h>
#include <math.h>
#include <time.h>

#define DAYS_IN_YEAR 365
#define DAYS_IN_HALF_YEAR DAYS_IN_YEAR / 2  
#define EPS 1e-3
#define SECONDS_IN_DAY 86400

struct Software* filter(const struct Software* info, uint* n, int (*pred)(const struct Software*)) {
    VERIFY_OR(info != NULL && n != NULL && pred != NULL, NULL, "Got null pointer as input");

    int* indices = (int*)malloc(*n * sizeof(int));
    VERIFY_OR(indices != NULL, NULL, "Could not allocate memory with malloc");
    for (int i = 0; i < *n; ++i) {
        indices[i] = -1;
    }

    int index_ptr = 0;
    for (int i = 0; i < *n; ++i) {
        int ret = pred(&info[i]);
        VERIFY_OR_WITH_CALLBACK(ret != -1, NULL, "Error during filtering check",
            free(indices));
        if (ret == 1) {
            indices[index_ptr++] = i;
        }
    }

    struct Software* filtered = (struct Software*)malloc(index_ptr * sizeof(struct Software));
    VERIFY_OR_WITH_CALLBACK(filtered != NULL, NULL, "Could not allocate memory with malloc",
        free(indices));

    for (int i = 0; i < index_ptr; ++i) {
        init_software_info(&filtered[i]);
    }
    for (int i = 0; i < index_ptr; ++i) {
        int ret = copy_software_info(&info[indices[i]], &filtered[i]);
        VERIFY_OR_WITH_CALLBACK(ret != -1, NULL, "Could not copy info", 
            { free(indices); free_software_array(filtered, index_ptr); });
    }
    free(indices);
    *n = index_ptr;
    return filtered;
}

int check(const struct Software* info) {
    VERIFY_OR(info != NULL, -1, "Got null pointer as input");
    int updates_ret = check_updates(&info->last_update_date, &info->installation_date);
    int installation_ret = check_installation_date(&info->installation_date, DAYS_IN_HALF_YEAR);
    VERIFY_OR(updates_ret != -1 && installation_ret != -1, -1, "Could not allocate memory with malloc");
    if (updates_ret == 1 && installation_ret == 1) {
        return 1;
    }
    return 0;
}

int check_updates(const struct Date* update_date, const struct Date* install_date) {
    VERIFY_OR(update_date != NULL && install_date != NULL, -1, "Got null pointer as input");
    return fabs(compare_dates(update_date, install_date)) < EPS;
}

int check_installation_date(const struct Date* date, uint days) {
    VERIFY_OR(date != NULL, -1, "Got null pointer as input");
    time_t time_now;
    time(&time_now);

    time_t time_half_year_ago = time_now - (DAYS_IN_HALF_YEAR * SECONDS_IN_DAY);
    time_t seconds_since_epoch = convert_to_time(date);

    double diff = difftime(seconds_since_epoch, time_half_year_ago);
    return diff >= 0;
}
