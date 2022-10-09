#include <stdio.h>

#ifndef MACROS_H
#define MACROS_H
#define VERIFY_OR(condition, return_value, error_msg) {\
    if (!(condition)) {\
        fprintf(stderr, "Error: %s, in file %s, line %d\n", error_msg, __FILE__, __LINE__);\
        return return_value;\
    }\
}
#define VERIFY_OR_WITH_CALLBACK(condition, return_value, error_msg, callback) {\
    if (!(condition)) {\
        fprintf(stderr, "Error: %s, in file %s, line %d\n", error_msg, __FILE__, __LINE__);\
        callback;\
        return return_value;\
    }\
}
#define EXIT_IF(condition, error_msg) {\
    if (condition) {\
        fprintf(stderr, "Error: %s, in file %s, line %d\n", error_msg, __FILE__, __LINE__);\
        exit(1);\
    }\
}
#define EXIT_IF_WITH_CALLBACK(condition, error_msg, callback) {\
    if (condition) {\
        fprintf(stderr, "Error: %s, in file %s, line %d\n", error_msg, __FILE__, __LINE__);\
        callback;\
        exit(1);\
    }\
}
#endif
