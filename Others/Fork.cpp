#include <unistd.h>
#include <iostream>

using namespace std;

int main() {
    pid_t c_pid = fork();
    if (c_pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (c_pid > 0) {
        cout << "Printed from parent process: " << getpid() << endl;;
    }
    else {
        cout << "Printed from child process: " << getpid() << endl;
    }
    return 0;
}