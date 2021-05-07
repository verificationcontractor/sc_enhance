#include <systemc>
#include "sc_enhance.hpp"
using namespace sc_core;
using namespace sc_dt;

SC_MODULE(demo) {
    SC_PROCESS_UTILS(demo); // some utilities needed to make our macros work

    sc_in<bool> clk {"clk"};
    sc_in<bool> reset {"reset"};
    int count = 0;
    bool run = false;

    // Declare method with sensitivities in the same place
    SC_METHOD_DECL(count_logic, [&](){ sensitive << clk.pos(); } ) {
        if(reset.read() == 1) {
            count = 0;
            run = true;
        }
        else if(run)
            count++;
    }

    // Declare thread with sensitivities in the same place
    SC_THREAD_DECL(test_thread, [&]() { 
      dont_initialize();
      sensitive << clk.pos();}) 
    {
        while(1) {
            if(count % 2 == 0) std::cout << "Counter is even." << std::endl;
            wait(clk.posedge_event());
        }
    }

    // Declare clocked thread with sensitivities in the same place
    SC_CTHREAD_DECL(test_cthread, clk.pos())
    {
        while(1) {
            if(count % 2 == 1) std::cout << "Counter is odd." << std::endl;
            wait();
        }
    }

    SC_CONS(demo) {
        // no need to register processes in constructor
        // no need to declare lists of sensitivities in constructor
    }
};

SC_MODULE(demo_tb) {
    SC_PROCESS_UTILS(demo_tb);

    sc_clock clk {"clk", sc_time(5, SC_NS), 0.5, SC_ZERO_TIME, false};
    sc_signal_decl(bool, reset);

    sc_instance(demo, demo_inst);
    CONNECT_PORTS_BEGIN
        demo_inst.clk(clk);
        demo_inst.reset(reset);
    CONNECT_PORTS_END

    SC_THREAD_DECL(logic, [&](){})
    {
      reset = 0;
      sc_repeat(2) wait(clk.posedge_event());
      reset = 1;
      sc_repeat(2) wait(clk.posedge_event());
      reset = 0;
      
      sc_repeat(100) wait(clk.posedge_event());

      sc_stop();

    }

    SC_CONS(demo_tb) {}
};

int sc_main(int argc, char** argv) {
    demo_tb demo_tb_inst("demo_tb_inst");
    sc_start();
    return 0;
}
