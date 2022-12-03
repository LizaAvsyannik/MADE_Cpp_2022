[![codecov](https://codecov.io/gh/LizaAvsyannik/MADE_Cpp_2022/branch/hw4/graph/badge.svg?token=NX85UGLETP)](https://codecov.io/gh/LizaAvsyannik/MADE_Cpp_2022)
### TO GET STARTED
```
cmake . -B build
cmake --build build
```
### CMAKE FLAGS
**WITH_COVERAGE** - to generate coverage report
**SANITIZE** - to enable sanitizers
**WITH_CLANG_TIDY** - to enable clang-tidy
**THREADED_IMPL** - to enable threaded implementation

### TO RUN PERFOMANCE TEST (Serial vs Parallel implementation)
```
./run_perfomance.sh
```

### ABOUT
В оперативную память загружен файл размером 100 Мб. Необходимо подсчитать количество пар соседних байт, разность значений которых равна 0, 1, 2, …, 10. Составьте наивный алгоритм подсчета, который сначала ищет все пары с нулевой разностью, затем — с единичной и т.д., а затем реализуйте параллельную обработку текста несколькими потоками с учетом оптимизации работы с кэш-памятью.
