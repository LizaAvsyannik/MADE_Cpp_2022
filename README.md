[![codecov](https://codecov.io/gh/LizaAvsyannik/MADE_Cpp_2022/branch/hw3/graph/badge.svg?token=NX85UGLETP)](https://codecov.io/gh/LizaAvsyannik/MADE_Cpp_2022)
### TO GET STARTED
```
cmake . -B build
cmake --build build
```
### CMAKE FLAGS
**WITH_COVERAGE** - to generate coverage report
**SANITIZE** - to enable sanitizers
**WITH_CLANG_TIDY** - to enable clang-tidy

### TO RUN TESTS (WITH COVERAGE REPORT)
```
cmake . -B build -DWITH_COVERAGE=ON -DSANITIZE=ON
cmake --build build
cd build/tests
ctest
cd ..
lcov --include <full_path_project>/src/\*  -t tests/ -d tests/ -c -o coverage.info
genhtml -o report coverage.info
```

### TO RUN PERFOMANCE TEST (COMPARISON WITH STL SET<T>)
```
cmake . -B build
cmake --build build
build/hw3_perfomance
```

### ABOUT
Analogue of STL Set<T> implemented using Red Black Tree.
