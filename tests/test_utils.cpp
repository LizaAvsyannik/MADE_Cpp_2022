#include <cstdlib>
#include <gtest/gtest.h>

extern "C" {
#include "impl/utils.h"
}

TEST(UTILS, CheckTrue) {
  struct Software test_info;
  test_info.name = (char *)malloc((MAX_NAME_LENGTH + 1) * sizeof(char));
  strcpy(test_info.name, "software_1"); // NOLINT (strcpy is unsafe)
  test_info.func_class = kDriver;
  test_info.version = {1, 2, 3, 4};
  test_info.installation_date = {15, 10, 2022};
  test_info.last_update_date = {15, 10, 2022};

  EXPECT_TRUE(check(&test_info));
  free_software(&test_info);
}

TEST(UTILS, MoreThanHalfYear) {
  struct Software test_info;
  test_info.name = (char *)malloc((MAX_NAME_LENGTH + 1) * sizeof(char));
  strcpy(test_info.name, "software_1"); // NOLINT (strcpy is unsafe)
  test_info.func_class = kDriver;
  test_info.version = {1, 2, 3, 4};
  test_info.installation_date = {15, 10, 2000};
  test_info.last_update_date = {15, 10, 2000};

  EXPECT_FALSE(check(&test_info));
  free_software(&test_info);
}

TEST(UTILS, Updated) {
  struct Software test_info;
  test_info.name = (char *)malloc((MAX_NAME_LENGTH + 1) * sizeof(char));
  strcpy(test_info.name, "software_1"); // NOLINT (strcpy is unsafe)
  test_info.func_class = kDriver;
  test_info.version = {1, 2, 3, 4};
  test_info.installation_date = {15, 10, 2022};
  test_info.last_update_date = {16, 10, 2022};

  EXPECT_FALSE(check(&test_info));
  free_software(&test_info);
}

TEST(UTILS, CheckFalse) {
  struct Software test_info;
  test_info.name = (char *)malloc((MAX_NAME_LENGTH + 1) * sizeof(char));
  strcpy(test_info.name, "software_1"); // NOLINT (strcpy is unsafe)
  test_info.func_class = kDriver;
  test_info.version = {1, 2, 3, 4};
  test_info.installation_date = {15, 10, 2000};
  test_info.last_update_date = {16, 10, 2001};

  EXPECT_FALSE(check(&test_info));
  free_software(&test_info);
}
