#include "impl/logic.h"
#include "impl/macros.h"

#include <stdio.h>
#include <string.h>

int main(int argc, const char* argv[]) {
    EXIT_IF(argc == 1, "Please provide input path");
    if (argc == 2) {
        process_input_file(argv[1], "output.txt");
    } else {
        process_input_file(argv[1], argv[2]);
    }
    return 0;
}
