
#include <systemc>
#include "sc_enhance.hpp"

using namespace sc_core;
using namespace sc_dt;

SC_MODULE(producer) {
    SC_PROCESS_UTILS(producer);

    sc_out_decl(bool, out_port);

    SC_THREAD_DECLARE(logic)
    SC_THREAD_BEGIN
      bool x = false;
      out_port.write(0);
      sc_repeat(5) {
          out_port.write(x);
          x = !x;
          wait(5, SC_NS);
      }
      sc_stop();
    SC_THREAD_END

    SC_CONS(producer) {}
};

SC_MODULE(consumer) {
    SC_PROCESS_UTILS(consumer);

    sc_in_decl(bool, in_port);

    SC_METHOD_DECLARE(logic)
      sensitive << in_port.pos();
    SC_METHOD_BEGIN
      std::cout << "Found posedge." << std::endl;
    SC_METHOD_END

    SC_CONS(consumer) {}
};

SC_MODULE(tb) {
    SC_PROCESS_UTILS(tb);

    sc_signal_decl(bool, signal);

    sc_instance(producer, prod);
    sc_instance(consumer, cons);
    CONNECT_PORTS_BEGIN
      prod.out_port(signal);
      cons.in_port(signal);
    CONNECT_PORTS_END

    SC_CONS(tb) {}
};

int sc_main(int argc, char** argv) {
  tb tb_inst("tb_inst");
  sc_start();
  return 0;
}
