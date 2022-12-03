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
Example from 20 threads:

![Screenshot from 2022-12-03 17-04-51](https://user-images.githubusercontent.com/66686119/205444984-61c27b02-4e58-4041-a803-ab9c8173e711.png)


### ABOUT
В оперативную память загружен файл размером 100 Мб. Необходимо подсчитать количество пар соседних байт, разность значений которых равна 0, 1, 2, …, 10. Составьте наивный алгоритм подсчета, который сначала ищет все пары с нулевой разностью, затем — с единичной и т.д., а затем реализуйте параллельную обработку текста несколькими потоками с учетом оптимизации работы с кэш-памятью.
