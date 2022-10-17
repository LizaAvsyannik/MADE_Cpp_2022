#include <cstdlib>
#include <gtest/gtest.h>
#include <stdio.h>
#include <string.h>

extern "C" {
#include "impl/data.h"
#include "impl/io.h"
}

TEST(IO_INPUT, FileInput) {
  char test_data[] = "2\nsoftware_1\n1 1 1 3 4\n16 07 2022\n16 07 "
                     "2022\nsoftware_2\n2 1 5 18 2\n02 02 2001\n16 07 2022\n";

  FILE *file = fmemopen(test_data, strlen(test_data), "r");
  uint n = 0;

  struct Software *test_info = read_software_info(file, &n);
  EXPECT_EQ(n, 2);

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

  EXPECT_EQ(strcmp(test_info[1].name, "software_2"), 0);
  EXPECT_EQ(test_info[1].func_class, kDriver);
  EXPECT_EQ(test_info[1].version.major, 1);
  EXPECT_EQ(test_info[1].version.minor, 5);
  EXPECT_EQ(test_info[1].version.patch, 18);
  EXPECT_EQ(test_info[1].version.build, 2);
  EXPECT_EQ(test_info[1].installation_date.day, 2);
  EXPECT_EQ(test_info[1].installation_date.month, 2);
  EXPECT_EQ(test_info[1].installation_date.year, 2001);
  EXPECT_EQ(test_info[1].last_update_date.day, 16);
  EXPECT_EQ(test_info[1].last_update_date.month, 07);
  EXPECT_EQ(test_info[1].last_update_date.year, 2022);

  fclose(file);
  free_software_array(test_info, n);
}

TEST(IO_INPUT, NullPointers) {
  char test_data[] = "2\nsoftware_1\n1 1 1 3 4\n16 07 2022\n16 07 2022\n"
                     "software_2\n2 1 5 18 2\n02 02 2001\n16 07 2022\n";

  FILE *file = fmemopen(test_data, strlen(test_data), "r");
  uint n = 0;

  struct Software *test_info = read_software_info(file, NULL);
  EXPECT_TRUE(test_info == NULL);

  test_info = read_software_info(NULL, &n);
  EXPECT_TRUE(test_info == NULL);

  test_info = read_software_info(file, NULL);
  EXPECT_TRUE(test_info == NULL);

  fclose(file);
}

TEST(IO_INPUT, EmptyFile) {
  // fmemopen for some reason fails when len is 0 even though man fmemopen
  // claims otherwise
  char test_data[] = " ";

  FILE *file = fmemopen(test_data, strlen(test_data), "r");
  uint n = 0;

  struct Software *test_info = read_software_info(file, &n);
  EXPECT_TRUE(test_info == NULL);

  fclose(file);
}

TEST(IO_INPUT, WrongInput) {
  // missing func class
  char test_data_1[] = "2\nsoftware_1\n 1 1 3 4\n16 07 2022\n16 07 "
                       "2022\nsoftware_2\n2 1 5 18 2\n02 02 2001\n16 07 2022\n";

  FILE *file = fmemopen(test_data_1, strlen(test_data_1), "r");
  uint n = 0;

  struct Software *test_info = read_software_info(file, &n);
  EXPECT_TRUE(test_info == NULL);

  fclose(file);

  // 07 -> July
  char test_data_2[] = "2\nsoftware_1\n1 1 1 3 4\n16 july 2022\n16 07 2022\n"
                       "software_2\n2 1 5 18 2\n02 02 2001\n16 07 2022\n";

  file = fmemopen(test_data_2, strlen(test_data_2), "r");

  test_info = read_software_info(file, &n);
  EXPECT_TRUE(test_info == NULL);

  fclose(file);
}

TEST(IO_OUTPUT, FileOuput) {
  struct Software test_info;
  test_info.name = (char *)malloc((MAX_NAME_LENGTH + 1) * sizeof(char));
  strcpy(test_info.name, "software_1"); // NOLINT (strcpy is unsafe)
  test_info.func_class = kDriver;
  test_info.version = {1, 2, 3, 4};
  test_info.installation_date = {1, 1, 2000};
  test_info.last_update_date = {2, 2, 2001};

  const int BUF_SIZE = 256;
  char *output = (char *)malloc(BUF_SIZE * sizeof(char));
  FILE *file = fmemopen(output, BUF_SIZE, "w");
  print_software_info(&test_info, file);
  fclose(file);
  free(test_info.name);

  EXPECT_EQ(strcmp(output, "software_1\n2 1 2 3 4\n01 01 2000\n02 02 2001\n"),
            0);

  free(output);
}
