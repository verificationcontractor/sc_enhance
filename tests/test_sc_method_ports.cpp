
#include <systemc>
#include "sc_enhance.hpp"

using namespace sc_core;
using namespace sc_dt;

SC_MODULE(producer) {
    SC_PROCESS_UTILS(producer);

    bool is_on = false;
    bool is_stopped = false;
    int count = 0;

    sc_out_method_port_declare(data_out, void(int));

    sc_in_method_port_declare(start, void(void), [&]() {
        std::cout << "Counter started" << std::endl;
        is_on = true;
    });

    sc_in_method_port_declare(pause, void(void), [&]() {
        std::cout << "Counter paused" << std::endl;
        is_on = false;
    });

    sc_in_method_port_declare(stop, void(void), [&]() {
        std::cout << "Counter stopped" << std::endl;
        is_on = false;
        is_stopped = true;
    });

    sc_in_method_port_declare(reset, void(void), [&]() {
        std::cout << "Counter reset" << std::endl;
        is_on = false;
        count = 0;
    });

    SC_THREAD_DECLARE(run)
    SC_THREAD_BEGIN
        while(true) {
            if(is_on) {
                data_out(count);
                count++;
            }
            if(is_stopped) break;
            wait(5,SC_NS);
        }
        sc_stop();
    SC_THREAD_END

    SC_CONS(producer) {}
};

SC_MODULE(consumer) {
    SC_PROCESS_UTILS(consumer);

    int periods = 0;

    sc_out_method_port_declare(reset,  void(void));
    sc_out_method_port_declare(start,  void(void));
    sc_out_method_port_declare(stop,   void(void));
    sc_out_method_port_declare(pause, void(void));

    sc_in_method_port_declare(data_in, void(int), [&](int x) {
        std::cout << "Received value " << x << std::endl;
        if(periods == 5)  { 
            stop();
        }
        if(x == 10) {
            periods++;
            reset();
            start();
        }
    });

    SC_THREAD_DECLARE(run)
    SC_THREAD_BEGIN
        wait(10,SC_NS);
        start();
    SC_THREAD_END

    SC_CONS(consumer) {}
};

SC_MODULE(tb) {
    SC_PROCESS_UTILS(tb);

    sc_instance(producer, prod);
    sc_instance(consumer, cons);

    CONNECT_PORTS_BEGIN
        prod.data_out.connect(cons.data_in);
        cons.start.connect(prod.start);
        cons.stop.connect(prod.stop);
        cons.pause.connect(prod.pause);
        cons.reset.connect(prod.reset);
    CONNECT_PORTS_END

    SC_CONS(tb) {}
};

int sc_main(int argc, char** argv) {
    tb tb_inst("tb_inst");
    sc_start();
    return 0;
}
