// Disabled warnings.

#pragma warning(disable : 4005 4996)

#include "application.hpp"
#include "init.hpp"
#include "render.hpp"
#include "update.hpp"

void throwing_main() {
    auto a = Application();
    run(a);
}

int main() {
    try {
        throwing_main();
        return 0;
    } catch(const std::exception& e) {
        std::cerr << "std::exception: " << e.what() << std::endl;
    } catch(...) {
        std::cerr << "Unhandled exception." << std::endl;
    }
    return -1;
}
