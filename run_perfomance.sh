num_files=3
filename="large_data/100mb_"
extension=".txt"

echo "Downloading files..."
for i in $(seq 1 $num_files)
do
    if [ ! -f "$filename$i$extension" ]
    then
        echo "Downloading file $i..."
        if [ $i -eq 1 ]
        then
            gdown https://drive.google.com/uc?id=12LO9zoM5y5GWlkJVE3xWcjN0MiuZbyEH 
        fi
        if [ $i -eq 2 ]
        then
            gdown https://drive.google.com/uc?id=1B6UtQGKjNUer9snh5MCMLCYYBoP3zbSJ 
        fi
        if [ $i -eq 3 ]
        then
            gdown https://drive.google.com/uc?id=1rhcU_VpcqP_if9x7H8B8hmdmkmkFWcCz 
        fi
        echo "File $i downloaded"
    fi
done
echo "Files downloaded"

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

for i in $(seq 1 $num_files)
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
