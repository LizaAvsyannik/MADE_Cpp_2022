#include "data.h"

#include <stdbool.h>

#ifndef UTILS_H
#define UTILS_H

struct Software* filter(const struct Software* info, uint* n, bool (*pred)(const struct Software*));

bool check_installation_date(const struct Date* date, uint days);

bool check_updates(const struct Date* update_date, const struct Date* install_date);

bool check(const struct Software* item);

#endif
