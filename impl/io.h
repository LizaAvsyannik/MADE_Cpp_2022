#include "data.h"

#include <stdio.h>

#ifndef IO_H
#define IO_H

struct Software *read_software_info(FILE *file, uint *num_of_soft);

int print_software_info(const struct Software *info, FILE *file);

#endif
