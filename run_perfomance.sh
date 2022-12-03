echo "Building parallel implementation..."
rm -rf build > /dev/null 2>&1
cmake -B build -DTHREADED_IMPL=ON > /dev/null 2>&1
cmake --build build > /dev/null 2>&1
echo "Parallel implementation built"
build/hw4_perfomance_measure

echo "Building serial implementation..."
rm -rf build > /dev/null 2>&1
cmake -B build > /dev/null 2>&1
cmake --build build > /dev/null 2>&1
build/hw4_perfomance_measure
echo "serial implementation built"

num_files=3
output_parallel="large_data/output_parallel_100mb_"
output_serial="large_data/output_serial_100mb_"
extension=".txt"

for i in $(seq 1 $END)
do 
    difference=$(diff "$output_parallel$i$extension" "$output_serial$i$extension" | wc -l)
    
    if [ $difference -ne 0 ]
    then
        echo "Different ouputs from parallel and serial implementations!"
        exit 1
    else
        echo "Same ouputs from parallel and serial implementations"
    fi
done
