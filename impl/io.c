#include "io.h"

#include <stdlib.h>
#include <string.h>

#include "data.h"
#include "macros.h"

int read_single_software_info(struct Software *item, FILE *file) {
  VERIFY_OR(item != NULL && file != NULL, -1, "Got null pointer as input");

  item->name = (char *)malloc((MAX_NAME_LENGTH + 1) * sizeof(char));
  VERIFY_OR(item->name != NULL, -1, "Could not allocate memory with malloc");

  char *str_or_null = fgets(item->name, MAX_NAME_LENGTH, file);
  VERIFY_OR(str_or_null != NULL, -1, "EOF during read");

  int str_length = strlen(item->name);
  if (item->name[str_length - 1] == '\n') {
    item->name[str_length - 1] = '\0';
  }

  uint func_class = 0;
  int ret =
      fscanf(file, "%u %u %u %u %u\n", &func_class, // NOLINT (fscanf is unsafe)
             &item->version.major, &item->version.minor, &item->version.patch,
             &item->version.build);
  item->func_class = (enum FunctionalClass)func_class;
  VERIFY_OR(ret != EOF && ret == 5, -1, "EOF during read");

  ret = fscanf(file, "%u %u %u\n", // NOLINT (fscanf is unsafe)
               &item->installation_date.day, &item->installation_date.month,
               &item->installation_date.year);
  VERIFY_OR(ret != EOF && ret == 3, -1, "EOF during read");

  ret = fscanf(file, "%u %u %u\n", // NOLINT (fscanf is unsafe)
               &item->last_update_date.day, &item->last_update_date.month,
               &item->last_update_date.year);
  VERIFY_OR(ret != EOF && ret == 3, -1, "EOF during read");
  return 0;
}

struct Software *read_software_info(FILE *file, uint *n) {
  VERIFY_OR(file != NULL && n != NULL, NULL, "Got null pointer as input");

  int ret = fscanf(file, "%u\n", n); // NOLINT (fscanf is unsafe)
  VERIFY_OR(ret != EOF, NULL, "EOF during read");

  struct Software *info =
      (struct Software *)malloc(*n * sizeof(struct Software));
  VERIFY_OR(info != NULL, NULL, "Could not allocate memory with malloc");
  for (uint i = 0; i < *n; ++i) {
    init_software_info(&info[i]);
  }
  for (uint i = 0; i < *n; ++i) {
    int ret_read = read_single_software_info(&info[i], file);
    VERIFY_OR_WITH_CALLBACK(ret_read != -1, NULL, "Could not read from file",
                            free_software_array(info, *n));
  }
  return info;
}

int print_software_info(const struct Software *info, FILE *file) {
  VERIFY_OR(info != NULL && file != NULL, -1, "Got null pointer as input");

  int ret = fprintf(file, "%s\n%d %u %u %u %u\n", info->name, info->func_class,
                    info->version.major, info->version.minor,
                    info->version.patch, info->version.build);
  VERIFY_OR(ret >= 0, -1, "Could not write to file");

  ret = fprintf(file, "%02u %02u %02u\n", info->installation_date.day,
                info->installation_date.month, info->installation_date.year);
  VERIFY_OR(ret >= 0, -1, "Could not write to file");

  ret = fprintf(file, "%02u %02u %02u\n", info->last_update_date.day,
                info->last_update_date.month, info->last_update_date.year);
  VERIFY_OR(ret >= 0, -1, "Could not write to file");

  return 0;
}
