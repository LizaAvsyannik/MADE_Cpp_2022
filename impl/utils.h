#include "data.h"

#include <stdbool.h>

#ifndef UTILS_H
#define UTILS_H

struct Software* filter(const struct Software* info, uint* n, int (*pred)(const struct Software*));

int check_installation_date(const struct Date* date, uint days);

int check_updates(const struct Date* update_date, const struct Date* install_date);

int check(const struct Software* item);

#endif
