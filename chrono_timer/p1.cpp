// In this program, we compute a sum from 0 to input-1 and 
// use chrono_timer to know the the executing time of the sum loop


#include <iostream>
#include <chrono>
#include <unistd.h>
#include <stdlib.h>

int main (int ac, char *av[]) {
    if (ac > 1) {
        unsigned long long sum = 0;
        unsigned long long length = atoi(av[1]);
        std::chrono::time_point<std::chrono::high_resolution_clock> start_time = std::chrono::high_resolution_clock::now();
        for (unsigned long long i = 0; i < length; i++) {
            sum += i;
        }
        std::chrono::time_point<std::chrono::high_resolution_clock> end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end_time - start_time;
        std::cout << " Problem size is : " << length << " " << std::endl;
        std::cout << " Elapsed time is : " << elapsed.count() << " " << std::endl;
        std::cout << " Total sum is : " << sum << " " << std::endl;
        return 0;
    }
    return -1;
}

