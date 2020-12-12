#include <systemc>
#include "sc_enhance.hpp"

using namespace sc_core;
using namespace sc_dt;

SC_MODULE(dut) {
    SC_PROCESS_UTILS(dut);

    int m;
    int n;

    SC_THREAD_DECLARE(logic)
    SC_THREAD_BEGIN
      sc_repeat(m) { 
          std::cout << name() << ": Waiting " << n << " nanoseconds" << std::endl;
          wait(n, SC_NS); 
          std::cout << name() << ": Done." << std::endl;
      }
    SC_THREAD_END

    SC_CONS(dut) {
        m=5; n=1;
    }

    SC_CONS(dut, int n) {
        m=5; this->n = n;
    }

    SC_CONS_EMPTY(dut, int m, int n);

};

SC_CONS_IMPLEMENT(dut, int m, int n) {
    this->m = m;
    this->n = n;
}

SC_MODULE(tb) {
    SC_PROCESS_UTILS(tb);

    sc_instance(dut, dut23, 2, 3);
    sc_instance(dut, dut52, 2);
    sc_instance(dut, dut51);

    SC_THREAD_DECLARE(logic)
    SC_THREAD_BEGIN
      wait(1000, SC_NS);
      sc_stop();
    SC_THREAD_END

    SC_CONS(tb) {}
};

int sc_main(int argc, char** argv) {
    tb tb_inst("tb_inst");
    sc_start();
    return 0;
}
