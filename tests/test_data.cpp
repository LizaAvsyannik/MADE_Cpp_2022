#include <cstdlib>
#include <gtest/gtest.h>

extern "C" {
    #include "impl/data.h"
}

TEST(DATA, Copy) {
    struct Software test_info;
    test_info.name = (char*)malloc((MAX_NAME_LENGTH + 1) * sizeof(char));
    strcpy(test_info.name, "software_1");
    test_info.func_class = kDriver;
    test_info.version = {1, 2, 3, 4};
    test_info.installation_date = {1, 1, 2000};
    test_info.last_update_date = {2, 2, 2001};

    struct Software test_info_copy;
    init_software_info(&test_info_copy);
    copy_software_info(&test_info, &test_info_copy);

    EXPECT_NE(test_info.name, test_info_copy.name);
    EXPECT_EQ(strcmp(test_info.name, test_info_copy.name), 0);

    EXPECT_EQ(test_info.func_class, test_info_copy.func_class);
    EXPECT_EQ(test_info.version.major, test_info_copy.version.major);
    EXPECT_EQ(test_info.version.minor, test_info_copy.version.minor);
    EXPECT_EQ(test_info.version.patch, test_info_copy.version.patch);
    EXPECT_EQ(test_info.version.build, test_info_copy.version.build);
    EXPECT_EQ(test_info.installation_date.day, test_info_copy.installation_date.day);
    EXPECT_EQ(test_info.installation_date.month, test_info_copy.installation_date.month);
    EXPECT_EQ(test_info.installation_date.year, test_info_copy.installation_date.year);
    EXPECT_EQ(test_info.last_update_date.day, test_info_copy.last_update_date.day);
    EXPECT_EQ(test_info.last_update_date.month, test_info_copy.last_update_date.month);
    EXPECT_EQ(test_info.last_update_date.year, test_info_copy.last_update_date.year);

    free_software(&test_info);
    free_software(&test_info_copy);
}

TEST(DATA, CompareDates) {
    const int SECONDS_IN_DAY = 86400;
    struct Date date1 = {15, 10, 2022}; 
    struct Date date2 = {15, 10, 2022};
    
    EXPECT_DOUBLE_EQ(compare_dates(&date1, &date2), 0);

    date2 = {15, 9, 2022};
    EXPECT_DOUBLE_EQ(compare_dates(&date1, &date2), 30 * SECONDS_IN_DAY);
}
