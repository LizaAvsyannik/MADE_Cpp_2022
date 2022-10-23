#include <time.h>

#ifndef DATA_H
#define DATA_H

#define MAX_NAME_LENGTH 30

typedef unsigned int uint;

enum FunctionalClass { kLibrary, kSystemPackage, kDriver, kMobileApp };

struct Version {
  uint major;
  uint minor;
  uint patch;
  uint build;
};

struct Date {
  uint day;
  uint month;
  uint year;
};

struct Software {
  char *name;
  enum FunctionalClass func_class;
  struct Version version;
  struct Date installation_date;
  struct Date last_update_date;
};

void free_software(struct Software *item);

void free_software_array(struct Software *arr, uint n);

void init_software_info(struct Software *info);

int copy_software_info(const struct Software *info, struct Software *new_info);

time_t convert_to_time(const struct Date *date);

double compare_dates(const struct Date *date1, const struct Date *date2);

#endif
