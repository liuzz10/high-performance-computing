// In this program, we compute a sum of initialized value in vector
// and use chrono_timer to know the the executing time of the sum loop


#include <iostream>
#include <chrono>
#include <unistd.h>
#include <stdlib.h>
#include <vector>

int main (int ac, char *av[]) {
    if (ac > 1) {
        unsigned long long sum = 0;
        unsigned long long length = atoi(av[1]);
        std::vector<unsigned long long> vect(length);
        for (unsigned long long i = 0; i < length; i++) {
            vect[i] = lrand48() % length;
            // We don't want the value == index, otherwise the pointer will never point to another number when calculating sum
            while (vect[i] == i) {
                vect[i] = lrand48() % length;
            }
        }
        unsigned long long pointer = 0;
        std::chrono::time_point<std::chrono::high_resolution_clock> start_time = std::chrono::high_resolution_clock::now();
        for (unsigned long long i = 0; i < length; i++) {
            sum += vect[pointer];
            pointer = vect[pointer];
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

