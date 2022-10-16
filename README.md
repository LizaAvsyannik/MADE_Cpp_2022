```cmake . -B build -DWITH_COVERAGE=ON -DSANITIZE=ON
cmake --build build
cd build/tests
ctest
cd ..
lcov -t tests/ -d impl/ -c -o coverage.info
genhtml -o report coverage.info
```