[![codecov](https://codecov.io/gh/LizaAvsyannik/MADE_Cpp_2022/branch/main/graph/badge.svg?token=NX85UGLETP)](https://codecov.io/gh/LizaAvsyannik/MADE_Cpp_2022)
### _To run programm_
```
cmake . -B build
cmake --build build
build/hw1_main resources/software.txt <output_file_path>
```
Default output path: _./output.txt_
### _General usage_:
```
build/hw1_main <input_file_path> <output_file_path>
```
### _Cmake Flags_
**WITH_COVERAGE** - to generate coverage report
**SANITIZE** - to enable sanitizers
**WITH_CLANG_TIDY** - to enable clang-tidy

### _To run tests (with coverage report)_
```
cmake . -B build -DWITH_COVERAGE=ON -DSANITIZE=ON
cmake --build build
cd build/tests
ctest
cd ..
lcov -t tests/ -d impl/ -c -o coverage.info
genhtml -o report coverage.info
```
