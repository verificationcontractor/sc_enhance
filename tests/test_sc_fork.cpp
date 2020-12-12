
//#define SC_INCLUDE_DYNAMIC_PROCESSES
#include <systemc>
#include "sc_enhance.hpp"

using namespace sc_core;
using namespace sc_dt;

SC_MODULE(fork_test) {
    SC_PROCESS_UTILS(fork_test);

    sc_clock clk {"clk", sc_time(5, SC_NS), 0.5, SC_ZERO_TIME, false};

    SC_THREAD_DECLARE(fork_join_any_test_logic)
        //dont_initialize();
    SC_THREAD_BEGIN
        SC_FORK
            [&](){
                std :: cout << "SC_JOIN_ANY Thread1 start " << sc_time_stamp() << std::endl;
                wait(10, SC_NS);
                std :: cout << "SC_JOIN_ANY Thread1 end " << sc_time_stamp() << std::endl;
            },
            [&](){
                std :: cout << "SC_JOIN_ANY Thread2 start " << sc_time_stamp() << std::endl;
                wait(50, SC_NS);
                std :: cout << "SC_JOIN_ANY Thread2 end " << sc_time_stamp() << std::endl;
            }
        SC_JOIN_ANY
        wait(100, SC_NS);
        std::cout << "SC_JOIN_ANY Done at " << sc_time_stamp() << std::endl;
    SC_THREAD_END

    SC_THREAD_DECLARE(fork_join_first_test_logic)
        //dont_initialize();
    SC_THREAD_BEGIN
        SC_FORK
            [&](){
                std :: cout << "SC_JOIN_FIRST Thread1 start " << sc_time_stamp() << std::endl;
                wait(10, SC_NS);
                std :: cout << "SC_JOIN_FIRST Thread1 end " << sc_time_stamp() << std::endl;
            },
            [&](){
                std :: cout << "SC_JOIN_FIRST Thread2 start " << sc_time_stamp() << std::endl;
                wait(50, SC_NS);
                std :: cout << "SC_JOIN_FIRST Thread2 end " << sc_time_stamp() << std::endl;
            }
        SC_JOIN_FIRST
        wait(100, SC_NS);
        std::cout << "SC_JOIN_FIRST Done at " << sc_time_stamp() << std::endl;
    SC_THREAD_END

    SC_THREAD_DECLARE(fork_join_none_test_logic)
        //dont_initialize();
    SC_THREAD_BEGIN
        SC_FORK
            [&](){
                std :: cout << "SC_JOIN_NONE Thread1 start " << sc_time_stamp() << std::endl;
                wait(10, SC_NS);
                std :: cout << "SC_JOIN_NONE Thread1 end " << sc_time_stamp() << std::endl;
            },
            [&](){
                std :: cout << "SC_JOIN_NONE Thread2 start " << sc_time_stamp() << std::endl;
                wait(50, SC_NS);
                std :: cout << "SC_JOIN_NONE Thread2 end " << sc_time_stamp() << std::endl;
            }
        SC_JOIN_NONE
        wait(100, SC_NS);
        std::cout << "SC_JOIN_NONE Done at " << sc_time_stamp() << std::endl;
    SC_THREAD_END

    SC_THREAD_DECLARE(fork_join_test_logic)
        //dont_initialize();
    SC_THREAD_BEGIN
        SC_FORK
            [&](){
                std :: cout << "SC_JOIN Thread1 start " << sc_time_stamp() << std::endl;
                wait(10, SC_NS);
                std :: cout << "SC_JOIN Thread1 end " << sc_time_stamp() << std::endl;
            },
            [&](){
                std :: cout << "SC_JOIN Thread2 start " << sc_time_stamp() << std::endl;
                wait(50, SC_NS);
                std :: cout << "SC_JOIN Thread2 end " << sc_time_stamp() << std::endl;
            }
        SC_JOIN
        std::cout << "SC_JOIN Done at " << sc_time_stamp() << std::endl;
    SC_THREAD_END

    SC_CTHREAD_DECLARE(fork_join_first_cthread_test_logic, clk)
        //dont_initialize();
    SC_CTHREAD_BEGIN
        SC_CFORK(clk)
            [&](){
                std :: cout << "SC_CJOIN_FIRST CThread1 start at " << sc_time_stamp() << std::endl;
                wait(1);
                std :: cout << "SC_CJOIN_FIRST CThread1 end at " << sc_time_stamp() << std::endl;
            },
            [&](){
                std :: cout << "SC_CJOIN_FIRST CThread2 start at " << sc_time_stamp() << std::endl;
                wait(5);
                std :: cout << "SC_CJOIN_FIRST CThread2 end at " << sc_time_stamp() << std::endl;
            }
        SC_CJOIN_FIRST
        sc_repeat(100) wait();
        std::cout << "SC_CJOIN_FIRST CDone at " << sc_time_stamp() << std::endl;
    SC_CTHREAD_END

    SC_CTHREAD_DECLARE(fork_join_any_cthread_test_logic, clk)
        //dont_initialize();
    SC_CTHREAD_BEGIN
        SC_CFORK(clk)
            [&](){
                std :: cout << "SC_CJOIN_ANY CThread1 start at " << sc_time_stamp() << std::endl;
                wait(1);
                std :: cout << "SC_CJOIN_ANY CThread1 end at " << sc_time_stamp() << std::endl;
            },
            [&](){
                std :: cout << "SC_CJOIN_ANY CThread2 start at " << sc_time_stamp() << std::endl;
                wait(5);
                std :: cout << "SC_CJOIN_ANY CThread2 end at " << sc_time_stamp() << std::endl;
            }
        SC_CJOIN_ANY
        sc_repeat(100) wait();
        std::cout << "SC_CJOIN_ANY CDone at " << sc_time_stamp() << std::endl;
    SC_CTHREAD_END

    SC_CTHREAD_DECLARE(fork_join_none_cthread_test_logic, clk)
        //dont_initialize();
    SC_CTHREAD_BEGIN
        SC_CFORK(clk)
            [&](){
                std :: cout << "SC_CJOIN_NONE CThread1 start at " << sc_time_stamp() << std::endl;
                wait(1);
                std :: cout << "SC_CJOIN_NONE CThread1 end at " << sc_time_stamp() << std::endl;
            },
            [&](){
                std :: cout << "SC_CJOIN_NONE CThread2 start at " << sc_time_stamp() << std::endl;
                wait(5);
                std :: cout << "SC_CJOIN_NONE CThread2 end at " << sc_time_stamp() << std::endl;
            }
        SC_CJOIN_NONE
        sc_repeat(100) wait();
        std::cout << "SC_CJOIN_NONE CDone at " << sc_time_stamp() << std::endl;
    SC_CTHREAD_END

    SC_CTHREAD_DECLARE(fork_join_cthread_test_logic, clk)
        //dont_initialize();
    SC_CTHREAD_BEGIN
        SC_CFORK(clk)
            [&](){
                std :: cout << "SC_CJOIN CThread1 start at " << sc_time_stamp() << std::endl;
                wait(1);
                std :: cout << "SC_CJOIN CThread1 end at " << sc_time_stamp() << std::endl;
            },
            [&](){
                std :: cout << "SC_CJOIN CThread2 start at " << sc_time_stamp() << std::endl;
                wait(5);
                std :: cout << "SC_CJOIN CThread2 end at " << sc_time_stamp() << std::endl;
            }
        SC_CJOIN
        std::cout << "SC_CJOIN CDone at " << sc_time_stamp() << std::endl;
    SC_CTHREAD_END

    // Cthread in thread - NOT RECOMMENDED, DOESN'T WORK AS IT SHOULD
   //SC_THREAD_DECLARE(fork_join_first_thread_cthread_test_logic)
   //     //dont_initialize();
   // SC_THREAD_BEGIN
   //     SC_CFORK(clk)
   //         [&](){
   //             std :: cout << "SC_CJOIN_FIRST_in_thread CThread1 start at " << sc_time_stamp() << std::endl;
   //             wait(1);
   //             std :: cout << "SC_CJOIN_FIRST_in_thread CThread1 end at " << sc_time_stamp() << std::endl;
   //         },
   //         [&](){
   //             std :: cout << "SC_CJOIN_FIRST_in_thread  CThread2 start at " << sc_time_stamp() << std::endl;
   //             wait(5);
   //             std :: cout << "SC_CJOIN_FIRST_in_thread  CThread2 end at " << sc_time_stamp() << std::endl;
   //         }
   //     SC_CJOIN_FIRST
   //     sc_repeat(100) wait(1, SC_NS);
   //     std::cout << "SC_CJOIN_FIRST_in_thread CDone at " << sc_time_stamp() << std::endl;
   // SC_THREAD_END

   // SC_THREAD_DECLARE(fork_join_any_thread_cthread_test_logic)
   //     //dont_initialize();
   // SC_THREAD_BEGIN
   //     SC_CFORK(clk)
   //         [&](){
   //             std :: cout << "SC_CJOIN_ANY_in_thread  CThread1 start at " << sc_time_stamp() << std::endl;
   //             wait(1);
   //             std :: cout << "SC_CJOIN_ANY_in_thread CThread1 end at " << sc_time_stamp() << std::endl;
   //         },
   //         [&](){
   //             std :: cout << "SC_CJOIN_ANY_in_thread CThread2 start at " << sc_time_stamp() << std::endl;
   //             wait(5);
   //             std :: cout << "SC_CJOIN_ANY_in_thread CThread2 end at " << sc_time_stamp() << std::endl;
   //         }
   //     SC_CJOIN_ANY
   //     sc_repeat(100) wait(1, SC_NS);
   //     std::cout << "SC_CJOIN_ANY_in_thread CDone at " << sc_time_stamp() << std::endl;
   // SC_THREAD_END

   // SC_THREAD_DECLARE(fork_join_none_thread_cthread_test_logic)
   //     //dont_initialize();
   // SC_THREAD_BEGIN
   //     SC_CFORK(clk)
   //         [&](){
   //             std :: cout << "SC_CJOIN_NONE_in_thread CThread1 start at " << sc_time_stamp() << std::endl;
   //             wait(1);
   //             std :: cout << "SC_CJOIN_NONE_in_thread CThread1 end at " << sc_time_stamp() << std::endl;
   //         },
   //         [&](){
   //             std :: cout << "SC_CJOIN_NONE_in_thread CThread2 start at " << sc_time_stamp() << std::endl;
   //             wait(5);
   //             std :: cout << "SC_CJOIN_NONE_in_thread CThread2 end at " << sc_time_stamp() << std::endl;
   //         }
   //     SC_CJOIN_NONE
   //     sc_repeat(100) wait(1, SC_NS);
   //     std::cout << "SC_CJOIN_NONE_in_thread CDone at " << sc_time_stamp() << std::endl;
   // SC_THREAD_END

   // SC_THREAD_DECLARE(fork_join_thread_cthread_test_logic)
   //     //dont_initialize();
   // SC_THREAD_BEGIN
   //     SC_CFORK(clk)
   //         [&](){
   //             std :: cout << "SC_CJOIN_in_thread CThread1 start at " << sc_time_stamp() << std::endl;
   //             wait(1);
   //             std :: cout << "SC_CJOIN_in_thread CThread1 end at " << sc_time_stamp() << std::endl;
   //         },
   //         [&](){
   //             std :: cout << "SC_CJOIN_in_thread CThread2 start at " << sc_time_stamp() << std::endl;
   //             wait(5);
   //             std :: cout << "SC_CJOIN_in_thread CThread2 end at " << sc_time_stamp() << std::endl;
   //         }
   //     SC_CJOIN
   //     std::cout << "SC_CJOIN_in_thread CDone at " << sc_time_stamp() << std::endl;
   // SC_THREAD_END

    SC_THREAD_DECLARE(end_of_test)
    SC_THREAD_BEGIN
        wait(2000, SC_NS);
        sc_stop();
    SC_THREAD_END

    SC_CONS(fork_test) {}
};

int sc_main(int argc, char** argv) {
    fork_test fork_test_inst("fork_test_inst");
    sc_start();
    return 0;
}
