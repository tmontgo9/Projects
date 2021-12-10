#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include "omploop.hpp"
#include <chrono>
#include <vector>
#include <algorithm>

#ifdef __cplusplus
extern "C" {
#endif
void generateMergeSortData (int* arr, size_t n);
void checkMergeSortResult (int* arr, size_t n);
#ifdef __cplusplus
}
#endif

void merge(int start, int mid, int end, int arr[])
{
    std::vector<int> temp;
    int value = start, push = mid+ 1;
    while(value <= mid && push <= end) {
        if (arr[value] < arr[push]) temp.push_back(arr[value++]);
        else temp.push_back(arr[push++]);
    }
    while (value<= mid) temp.push_back(arr[value++]);
    for(int i = start, j = 0; j < temp.size(); i++, j++) {
        arr[i] = temp[j];
    }
}

int main (int argc, char* argv[]) {
    auto start = std::chrono::system_clock::now();
    if (argc < 3) { std::cerr<<"Usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
        return -1;
    }
    
    int n= atoi(argv[1]);
    
    // get arr data
    int * arr = new int [n];
    generateMergeSortData (arr, n);
    
    //insert sorting code here.
    OmpLoop loop;
    loop.setNbThread(atoi(argv[2]));
    
    for (int batch = 1; batch < n; batch = 2 * batch){
        loop.parfor<int>(
                         0, n, 2* batch,
                         [&](int & tls){},
                         [&](int i, int & tls){
                             int mid = std::min(i + batch- 1, n- 1);
                             int end = std::min(i + 2 * batch- 1, n- 1);
                             merge(i, mid, end, std::ref(arr));
                         },
                         [&](int & tls){}
                         );
    }
    
    checkMergeSortResult (arr, n);
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = end - start;
    std::cerr << diff.count() << std::endl;
    delete[] arr;
    
    return 0;
}
