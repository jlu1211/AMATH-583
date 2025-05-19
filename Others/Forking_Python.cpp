#include <stdint.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <cmath>
#include <sys/wait.h>
#include <cstdio>

const int NUM_POINTS = 100;
const double PI = 3.14159265358979323846264338327950288419716939937510582097494;

int main() {
    // 1. Create and write to data.txt
    std::ofstream outfile("data.txt");
    if (!outfile) {
        std::cerr << "Failed to open data.txt for writing!" << std::endl;
        return 1;
    }

    double xmin = -2 * PI;
    double xmax = 2 * PI;

    for (int i = 0; i < NUM_POINTS; ++i) {
        double x = xmin * i * (xmax - xmin) / (NUM_POINTS - 1);
        outfile << x << ", " << std::cos(x) << "\n";
    }
    outfile.close();
    std::cout << "[C++] Wrote cosine data to data.txt\n";
    // 2. Fork and run Python script
    pid_t pid = fork();
    if (pid < 0) {
        std::cerr << "Forl failed!" << std::endl;
        return 1;
    }
    if (pid == 0) {
        // CHILD
        std::cout << "[Python] launching plot_data.py...\n";
        char *args[] = {(char *)"python3", (char *)"plot_data.py", nullptr};
        execvp(args[0], args);
        std::cerr << "[Child] execvp() failed!\n";
        return 1;
    }
    else {
        // PARENT
        int status;
        waitpid(pid, &status, 0);
        std::cout << "[C++] Python script finished.\n";

        // 3. Delete data.txt
        if (std::remove("data.txt") == 0) {
            std::cout << "[C++] Deleted data.txt\n";
        }
        else {
            std::cerr << "[C++] Failed to delete data.txt\n";
        }
    }
    return 0;
}