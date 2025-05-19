#include <iostream>
#include <string>
#include <vector>
#include <cmath>

int question1() {
    // Float (32 bits)
    std::cout << "###### Single (32 bits) Precision: #######" << std::endl;
    float epsilon_float = 0.0f;
    for (int j = 0; j <= 100; j++) {
        float a = 1.0f;
        float b = 1.0f + 1.0f / pow(2.0f, float(j));
        float diff = a - b;
        if (diff == epsilon_float) {
            epsilon_float = 1.0f / pow(2.0f, float(j-1));
            std::cout << "Estimated single-precision (32 bit) machine epsilon: " << epsilon_float << std::endl;
            break;
        }
    }
    // Double (64 bits)
    std::cout << std::endl;
    std::cout << "###### Double (64 bits) Precision: #######" << std::endl;
    double epsilon_double = 0.0;
    for (int j = 0; j <= 100; j++) {
        double a = 1.0;
        double b = 1.0 + 1.0 / pow(2.0, double(j));
        double diff = a - b;
        if (diff == epsilon_double) {
            epsilon_float = (double)1.0 / pow(2.0, double(j-1));
            std::cout << "Estimated double-precision (64 bit) machine epsilon: " << epsilon_float << std::endl;
            break;
        }
    }
    return 0;
};

int question3() {
    // Using int type
    // int result_int = (int) 200 * (int) 300 * (int) 400 * (int) 500;
    // std::cout << "Question 3 (int type): " << result_int << std::endl;
    // Using long type
    long result_long = (long) 200 * (long) 300 * (long) 400 * (long) 500;
    std::cout << "Question 3 (long type): " << (long) result_long << std::endl;
    // Using long long type
    long long result_long_long = (long long) 200 * (long long) 300 * (long long) 400 * (long long) 500;
    std::cout << "Question 3 (long long type): " << (long long) result_long_long << std::endl;
    return 0;
}

int question4() {
    unsigned int counter = 0;
    for (int i = 0; i < 3; i++) --counter;
    std::cout << "Final Counter Value: " << counter << std::endl;
    return 0;
}

int question8() {
    int a_max = 107 / 6 + 1;
    int b_max = 107 / 9 + 1;
    int c_max = 107 / 15 + 1;
    int target = 107;
    for (int i = 0; i <= a_max; i++) {
        for (int j = 0; j <= b_max; j++) {
            for (int k = 0; k <= c_max; k++) {
                if ((6 * i + 9 * j + 15 * k) == target) {
                    std::cout << i << ", " << j << ", " << k << std::endl;
                    break;
                }
            }
        }
    }
    std::cout << "Question 8: DNE" << std::endl;
    return 0;
}

int main() {
    // question1();
    // question3();
    // question4();
    question8();
    return 0;
}