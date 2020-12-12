#include <systemc>
#include "sc_enhance.hpp"

using namespace sc_core;
using namespace sc_dt;
  
SC_MODULE(test_module) {
  SC_PROCESS_UTILS(test_module);

  sc_in_decl(bool, clk);

  int count = 0;

  // count_logic
  SC_METHOD_DECLARE(count_logic)
    sensitive << clk.pos();
  SC_METHOD_BEGIN
    std::cout << "count = " << count << std::endl;
    count++;
    if(count == 50) sc_stop();
  SC_METHOD_END // count_logic

  // test_thread
  SC_THREAD_DECLARE(test_thread) 
    dont_initialize();
    sensitive << clk.pos();
  SC_THREAD_BEGIN 
    wait(2, SC_NS);
    std::cout << "HELLO FROM THREAD!!!!!!" << std::endl;
  SC_THREAD_END // test_thread

  SC_CTHREAD_DECLARE(test_cthread, clk.pos())
  SC_CTHREAD_BEGIN
    wait(4);
    std::cout << "HELLO FROM CTHREAD!!!!!!" << std::endl;
  SC_CTHREAD_END // test_cthread

  SC_METHOD_DECLARE_EMPTY(count_monitor_logic);

  SC_THREAD_DECLARE_EMPTY(detect_clk_11);

  SC_CTHREAD_DECLARE_EMPTY(detect_clk_42);

  // constructor
  SC_CONS_EMPTY(test_module);

};

SC_CONS_IMPLEMENT(test_module) {
  
  // count_monitor_logic
  SC_METHOD(count_monitor_logic);
    sensitive << clk.pos() << clk.neg();
  SC_METHOD_IMPLEMENT(count_monitor_logic) {
    std::cout << "Incremented counter is:" << count + 1 << std::endl;
  };

  // detect_clk_11
  SC_THREAD(detect_clk_11);
    dont_initialize();
    sensitive << clk.pos();
  SC_THREAD_IMPLEMENT(detect_clk_11) {
    for(int i = 0; i < 11; i++)
      wait(clk.posedge_event());
    std::cout << "Reached clock 11." << std::endl;
  };

  // detect_clk_42
  SC_CTHREAD(detect_clk_42, clk.pos());
  SC_CTHREAD_IMPLEMENT(detect_clk_42) {
    for(int i = 0; i < 42; i++)
      wait();
    std::cout << "Reached clock 42." << std::endl;
  };

}

int sc_main(int argc, char** argv) {
  sc_clock ref_clk("ref_clk", sc_time(1, SC_NS), 0.5, SC_ZERO_TIME, true);
  test_module test("test");
  test.clk(ref_clk);
  sc_start();
  return 0;
}
