#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <exception>
#include <typeinfo>
#include <unordered_set>

// using namespace Helpers;

double solve(double time, double score) {
    double discriminant = time * time - 4 * score;
    double root1, root2;

    // time is positive => b is negative
    root1 = (time + sqrt(discriminant)) / (2);
    root2 = (time - sqrt(discriminant)) / (2);

    // finite move inwards to remove exact solutions
    root1 = std::nexttoward(root1, root2);
    root2 = std::nexttoward(root2, root1);

    double count = std::floor(root1) - std::ceil(root2) + 1;

    return count;
}

#include <iostream>
#include <cmath>

void quadraticFormula(double a, double b, double c) {
    double discriminant = b * b - 4 * a * c;
    double root1, root2;

    if (discriminant > 0) {
        root1 = (-b + sqrt(discriminant)) / (2 * a);
        root2 = (-b - sqrt(discriminant)) / (2 * a);
        std::cout << std::setprecision(std::numeric_limits<double>::max_digits10) << "Root 1 = " << root1 << std::endl;
        std::cout << std::setprecision(std::numeric_limits<double>::max_digits10) << "Root 2 = " << root2 << std::endl;
    } else if (discriminant == 0) {
        root1 = root2 = -b / (2 * a);
        std::cout << std::setprecision(std::numeric_limits<double>::max_digits10) << "Root 1 = Root 2 = " << root1 << std::endl;
    } else {
        double realPart = -b / (2 * a);
        double imaginaryPart = sqrt(-discriminant) / (2 * a);
        std::cout << "Root 1 = " << std::setprecision(std::numeric_limits<double>::max_digits10) << realPart << " + " << imaginaryPart << "i" << std::endl;
        std::cout << "Root 2 = " << std::setprecision(std::numeric_limits<double>::max_digits10) << realPart << " - " << imaginaryPart << "i" << std::endl;
    }
}

int main() {
    double a, b, c;
    a = 1;
    b = -61677571;
    c = 430103613071150;

    quadraticFormula(a, b, c);

    return 0;
}
