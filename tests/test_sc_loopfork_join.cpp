#include <systemc>
#include "sc_enhance.hpp"

using namespace sc_core;
using namespace sc_dt;

SC_MODULE(loopfork_test) {
    SC_PROCESS_UTILS(loopfork_test);

    sc_clock clk {"clk", sc_time(1, SC_NS), 0.5, SC_ZERO_TIME, false};

    SC_THREAD_DECLARE(test_fork_join_none)
        // dont_initialize();
    SC_THREAD_BEGIN
        sc_fork_list proc_list;
        for(int i = 0; i < 10; i++) {
            proc_list.push_back(sc_bind([&](int ii){
                std::cout<< "SC_JOIN_NONE in for-loop: Thread " << ii << " begin" << std::endl;
                wait(1, SC_NS);
                std::cout<< "SC_JOIN_NONE in for-loop: Thread " << ii << " end" << std::endl; 
            }, i));
        }
        SC_FORK_JOIN_NONE(proc_list);
        wait(1, SC_NS);
    SC_THREAD_END

    SC_THREAD_DECLARE(test_fork_join_none1)
        // dont_initialize();
    SC_THREAD_BEGIN
        for(int i = 0; i < 10; i++) {
            SC_FORK
                sc_bind([&](int ii) { 
                    std::cout<< "SC_JOIN_NONE1 in for-loop: Thread " << ii << " begin" << std::endl;
                    wait(1, SC_NS);
                    std::cout<< "SC_JOIN_NONE1 in for-loop: Thread " << ii << " end" << std::endl; 
                }, i),
            SC_JOIN_NONE
        }
        wait(1, SC_NS);
    SC_THREAD_END

    SC_THREAD_DECLARE(test_fork_join_first)
        // dont_initialize();
    SC_THREAD_BEGIN
        sc_fork_list proc_list;
        for(int i = 0; i < 10; i++) {
            proc_list.push_back(sc_bind([&](int ii){
                std::cout<< "SC_JOIN_FIRST in for-loop: Thread " << ii << " begin" << std::endl;
                wait(ii+1, SC_NS);
                std::cout<< "SC_JOIN_FIRST in for-loop: Thread " << ii << " end" << std::endl; 
            }, i));
        }
        SC_FORK_JOIN_FIRST(proc_list);
    SC_THREAD_END

    SC_THREAD_DECLARE(test_fork_join_any)
        // dont_initialize();
    SC_THREAD_BEGIN
        sc_fork_list proc_list;
        for(int i = 0; i < 10; i++) {
            proc_list.push_back(sc_bind([&](int ii){
                std::cout<< "SC_JOIN_ANY in for-loop: Thread " << ii << " begin" << std::endl;
                wait(ii+1, SC_NS);
                std::cout<< "SC_JOIN_ANY in for-loop: Thread " << ii << " end" << std::endl; 
            }, i));
        }
        SC_FORK_JOIN_ANY(proc_list);
        wait(1, SC_NS);
    SC_THREAD_END

    SC_THREAD_DECLARE(test_fork_join)
        // dont_initialize();
    SC_THREAD_BEGIN
        sc_fork_list proc_list;
        for(int i = 0; i < 10; i++) {
            proc_list.push_back(sc_bind([&](int ii){
                std::cout<< "SC_JOIN in for-loop: Thread " << ii << " begin" << std::endl;
                wait(ii, SC_NS);
                std::cout<< "SC_JOIN in for-loop: Thread " << ii << " end" << std::endl; 
            }, i));
        }
        SC_FORK_JOIN(proc_list);
    SC_THREAD_END

    // Clocked threads
    SC_CTHREAD_DECLARE(test_cfork_cjoin_none, clk)
        // dont_initialize();
    SC_THREAD_BEGIN
        sc_fork_list proc_list;
        for(int i = 0; i < 10; i++) {
            proc_list.push_back(sc_bind([&](int ii){
                std::cout<< "SC_CJOIN_NONE in for-loop: Thread " << ii << " begin" << std::endl;
                wait(1);
                std::cout<< "SC_CJOIN_NONE in for-loop: Thread " << ii << " end" << std::endl; 
            }, i));
        }
        SC_CFORK_CJOIN_NONE(proc_list, clk);
        wait(1);
    SC_THREAD_END

    SC_CTHREAD_DECLARE(test_cfork_cjoin_none1, clk)
        // dont_initialize();
    SC_THREAD_BEGIN
        for(int i = 0; i < 10; i++) {
            SC_CFORK(clk)
                sc_bind([&](int ii) { 
                    std::cout<< "SC_CJOIN_NONE1 in for-loop: Thread " << ii << " begin" << std::endl;
                    wait(1, SC_NS);
                    std::cout<< "SC_CJOIN_NONE1 in for-loop: Thread " << ii << " end" << std::endl; 
                }, i),
            SC_CJOIN_NONE
        }
        wait(1);
    SC_THREAD_END

    SC_CTHREAD_DECLARE(test_cfork_cjoin_first, clk)
        // dont_initialize();
    SC_THREAD_BEGIN
        sc_fork_list proc_list;
        for(int i = 0; i < 10; i++) {
            proc_list.push_back(sc_bind([&](int ii){
                std::cout<< "SC_CJOIN_FIRST in for-loop: Thread " << ii << " begin" << std::endl;
                wait(ii+1);
                std::cout<< "SC_CJOIN_FIRST in for-loop: Thread " << ii << " end" << std::endl; 
            }, i));
        }
        SC_CFORK_CJOIN_FIRST(proc_list, clk);
    SC_THREAD_END

    SC_CTHREAD_DECLARE(test_cfork_cjoin_any, clk)
        // dont_initialize();
    SC_THREAD_BEGIN
        sc_fork_list proc_list;
        for(int i = 0; i < 10; i++) {
            proc_list.push_back(sc_bind([&](int ii){
                std::cout<< "SC_CJOIN_ANY in for-loop: Thread " << ii << " begin" << std::endl;
                wait(ii+1);
                std::cout<< "SC_CJOIN_ANY in for-loop: Thread " << ii << " end" << std::endl; 
            }, i));
        }
        SC_CFORK_CJOIN_ANY(proc_list, clk);
        wait(9);
    SC_THREAD_END

    SC_THREAD_DECLARE(end_of_test)
    SC_THREAD_BEGIN
        wait(10, SC_NS);
        sc_stop();
    SC_THREAD_END

    SC_CONS(loopfork_test) {}

};

int sc_main(int argc, char** argv) {
    loopfork_test lft("lft");
    sc_start();
    return 0;
}
