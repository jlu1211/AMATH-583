#include <iostream>
#include <iomanip>
#include <string>

class rectangle{
    public:
        int x, y;
        int area();
        int perimeter();
        rectangle();
        ~rectangle();
};

rectangle::rectangle() {
    std::cout << "rectangle constructed" << std::endl;
};

rectangle::~rectangle() {
    std::cout << "rectangle destructed" << std::endl;
}

int rectangle::area() {
    return x * y;
}

int rectangle::area(int a, int b) {
    return a * b;
}

int rectangle::perimeter() {
    return 2 * x + 2 * y;
}

int main() {
    rectangle r1;
    r1.x = 4;
    r1.y = 5;
    std::cout << r1.area() << std::endl;
    std::cout << r1.perimeter() << std::endl;
    return 0;
}