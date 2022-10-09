#include "data.h"

#include <stdio.h>

#ifndef IO_H
#define IO_H

struct Software* read_software_info(const char* filename, uint* num_of_soft);

int read_single_software_info(struct Software* item, FILE* file);

int print_software_info(const struct Software* info, FILE* file);

#endif
