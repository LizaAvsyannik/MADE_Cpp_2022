#include <cstdlib>
#include <gtest/gtest.h>
#include <stdio.h>
#include <string.h>

extern "C" {
#include "impl/io.h"
#include "impl/logic.h"
}

TEST(LOGIC, Filter) {
  char test_data[] = "2\nsoftware_1\n1 1 1 3 4\n16 07 2022\n16 07 "
                     "2022\nsoftware_2\n2 1 5 18 2\n02 02 2001\n16 07 2022\n";
  FILE *input_file = fopen("input_file.txt", "w");
  fprintf(input_file, "%s", test_data);
  fclose(input_file);

  process_input_file("input_file.txt", "output_file.txt");

  FILE *output_file = fopen("output_file.txt", "r");

  uint n = 0;
  struct Software *test_info = read_software_info(output_file, &n);

  EXPECT_EQ(strcmp(test_info[0].name, "software_1"), 0);
  EXPECT_EQ(test_info[0].func_class, kSystemPackage);
  EXPECT_EQ(test_info[0].version.major, 1);
  EXPECT_EQ(test_info[0].version.minor, 1);
  EXPECT_EQ(test_info[0].version.patch, 3);
  EXPECT_EQ(test_info[0].version.build, 4);
  EXPECT_EQ(test_info[0].installation_date.day, 16);
  EXPECT_EQ(test_info[0].installation_date.month, 07);
  EXPECT_EQ(test_info[0].installation_date.year, 2022);
  EXPECT_EQ(test_info[0].last_update_date.day, 16);
  EXPECT_EQ(test_info[0].last_update_date.month, 07);
  EXPECT_EQ(test_info[0].last_update_date.year, 2022);

  fclose(output_file);
  free_software_array(test_info, n);

  remove("input_file.txt");
  remove("output_file.txt");
}
