import test_module;

#include <iostream>


int main() {
    std::cout << "class name: " << NTestModule::TPublicClass{}.Name() << std::endl;
}
