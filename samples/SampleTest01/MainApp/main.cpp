#include <iostream>
#include "module_math/math.h"
#include "module_user/user.h"

int main()
{
    std::cout << "1 + 2 = " << Add(1, 2) << std::endl;
    std::cout << "age 20 adult? " << CheckAdult(20) << std::endl;
    std::cout << "age 17 adult? " << CheckAdult(17) << std::endl;
    return 0;
}