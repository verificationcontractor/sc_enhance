
//#define SC_INCLUDE_DYNAMIC_PROCESSES
#include <systemc>
#include "sc_enhance.hpp"

using namespace sc_core;
using namespace sc_dt;

SC_MODULE(fork_test) {
    SC_PROCESS_UTILS(fork_test);

    int x = 7;

    sc_clock clk {"clk", sc_time(5, SC_NS), 0.5, SC_ZERO_TIME, false};

    SC_THREAD_DECLARE(fork_test_logic)
        //dont_initialize();
    SC_THREAD_BEGIN
        SC_FORK
            [&](){
                std :: cout << "Thread1 start " << sc_time_stamp() << std::endl;
                wait(10, SC_NS);
                std :: cout << "Thread1 end " << sc_time_stamp() << std::endl;
            },
            [&](){
                std :: cout << "Thread2 start " << sc_time_stamp() << std::endl;
                wait(50, SC_NS);
                std :: cout << "Thread2 end " << sc_time_stamp() << std::endl;
            }
        SC_JOIN_ANY
        wait(100, SC_NS);
        std::cout << "Done." << std::endl;
        sc_stop();
    SC_THREAD_END

    SC_CTHREAD_DECLARE(fork_cthread_test_logic, clk)
    //    dont_initialize();
    SC_CTHREAD_BEGIN
        SC_CFORK(clk)
            [&](){
                std::cout << "x = " << x << std::endl;
                std :: cout << "CThread1 start at " << sc_time_stamp() << std::endl;
                wait(1);
                std :: cout << "CThread1 end at " << sc_time_stamp() << std::endl;
            },
            [&](){
                std :: cout << "CThread2 start at " << sc_time_stamp() << std::endl;
                wait(5);
                std :: cout << "CThread2 end at " << sc_time_stamp() << std::endl;
            }
        SC_CJOIN_FIRST
        sc_repeat(100) wait();
        std::cout << "CDone at " << sc_time_stamp() << std::endl;
        sc_stop();
    SC_CTHREAD_END

    SC_CONS(fork_test) {}
};

int sc_main(int argc, char** argv) {
    fork_test fork_test_inst("fork_test_inst");
    sc_start();
    return 0;
}