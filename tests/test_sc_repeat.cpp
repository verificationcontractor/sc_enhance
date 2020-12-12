#include <systemc>
#include "sc_enhance.hpp"
using namespace sc_core;
using namespace sc_dt;

void test_sc_repeat(int m, int n) {
    int count = 0;
    sc_repeat(m) sc_repeat(n) count++;
    std::cout << "--" << std::endl;
    std::cout << "m=" << m << std::endl;
    std::cout << "n=" << n << std::endl;
    std::cout << "count=" << count << std::endl;
    std::cout << "--" << std::endl;
}

int sc_main(int argc, char** argv) {
    test_sc_repeat(0,1);
    test_sc_repeat(1,0);
    test_sc_repeat(1,5);
    test_sc_repeat(5,1);
    test_sc_repeat(50,100);
    test_sc_repeat(4,4);
    test_sc_repeat(6,2);
    test_sc_repeat(2,7);
    return 0;
}
