
#include <systemc>
#include "sc_enhance.hpp"

using namespace sc_core;
using namespace sc_dt;

SC_MODULE(shift_add_multiplier) {

    SC_PROCESS_UTILS(shift_add_multiplier);

    // Ports
    sc_in_decl(bool, clk);
    sc_in_decl(bool, reset);
    sc_in_decl(sc_bv<8>, in_data1);
    sc_in_decl(sc_bv<8>, in_data2);
    sc_in_decl(bool, in_data_valid);
    sc_out_decl(bool, busy);

    sc_out_decl(sc_bv<16>, out_data);
    sc_out_decl(bool, out_data_valid);

    // Registers
    sc_signal_decl(sc_uint<8>, data1);
    sc_signal_decl(sc_uint<16>, data2);
    sc_signal_decl(sc_uint<16>, result);
    sc_signal_decl(bool, is_busy);

    // adder inputs
    sc_signal_decl(sc_uint<16>, add_data1);
    sc_signal_decl(sc_uint<16>, add_data2);

    // Logic

    SC_METHOD_DECLARE(seq_multiplier_logic)
        sensitive << clk.pos();
    SC_METHOD_BEGIN
        if( reset.read() ) {
            data1 = 0;
            data2 = 0;
            add_data1 = 0;
            add_data2 = 0;
            is_busy = 0;
        }
        else if( in_data_valid.read() ) {
            data1 = in_data1.read();
            data2 = (sc_uint<8>(0x00), sc_uint<8>(in_data2.read()));
            add_data1 = 0;
            add_data2 = 0;
            is_busy = 1;
        } else if(is_busy.read()) {
            if(data1.read()[0] == 1) {
                //result = result.read() + data2.read();
                add_data1 = result.read();
                add_data2 = data2.read();
            }
            if (data1.read() != 0) {
                data1 = data1.read() >> 1;
                data2 = data2.read() << 1;
            } else {
                is_busy = 0;
                out_data_valid = 1;
                out_data = result.read();
                data1 = 0;
                data2 = 0;
                add_data1 = 0;
                add_data2 = 0;
            }
        } else {
            out_data_valid = 0;
            out_data = 0;
        }
    SC_METHOD_END

    SC_METHOD_DECLARE(adder)
        sensitive << add_data1 << add_data2;
    SC_METHOD_BEGIN
        result = add_data1.read() + add_data2.read();
    SC_METHOD_END
        
    SC_METHOD_DECLARE(connect_busy_output)
        sensitive << is_busy;
    SC_METHOD_BEGIN
        busy = is_busy.read();
    SC_METHOD_END

    SC_CONS(shift_add_multiplier) {}
};

SC_MODULE(shift_add_multiplier_tb) {
    SC_PROCESS_UTILS(shift_add_multiplier_tb);

    sc_clock clk {"clk", sc_time(5, SC_NS), 0.5, SC_ZERO_TIME, true};
    sc_signal_decl(bool, reset);
    sc_signal_decl(sc_bv<8>, in_data1);
    sc_signal_decl(sc_bv<8>, in_data2);
    sc_signal_decl(bool, in_data_valid);
    sc_signal_decl(bool, busy);

    sc_signal_decl(sc_bv<16>, out_data);
    sc_signal_decl(bool, out_data_valid);

    shift_add_multiplier dut {"dut"};
    CONNECT_PORTS_BEGIN
        dut.clk(clk);
        dut.reset(reset);
        dut.in_data1(in_data1);
        dut.in_data2(in_data2);
        dut.in_data_valid(in_data_valid);
        dut.busy(busy);
        dut.out_data_valid(out_data_valid);
        dut.out_data(out_data);
    CONNECT_PORTS_END

    SC_CTHREAD_DECLARE(my_test, clk)
    SC_CTHREAD_BEGIN
        // drive reset
        reset = 0;
        wait(5);
        reset = 1;
        wait(2);
        reset = 0;

        // drive data
        wait(5);
        in_data_valid = 1;
        in_data1 = 255;
        in_data2 = 255;
        wait(1);
        in_data_valid = 0;
        in_data1 = 0;
        in_data2 = 0;

        // wait for result
        wait(20);
        sc_stop();
    SC_CTHREAD_END

    sc_trace_file *Tf;

    SC_CONS(shift_add_multiplier_tb) {
        // Dump waves
        Tf = sc_create_vcd_trace_file("traces");
        sc_trace(Tf, clk, "clk");
        sc_trace(Tf, reset, "reset");
        sc_trace(Tf, in_data1, "in_data1");
        sc_trace(Tf, in_data2, "in_data2");
        sc_trace(Tf, in_data_valid, "in_data_valid");
        sc_trace(Tf, busy, "busy");
        sc_trace(Tf, out_data_valid, "out_data_valid");
        sc_trace(Tf, out_data, "out_data");

        sc_trace(Tf, dut.data1, "dut.data1");
        sc_trace(Tf, dut.data2, "dut.data2");
        sc_trace(Tf, dut.data1, "dut.add_data1");
        sc_trace(Tf, dut.data2, "dut.add_data2");
        sc_trace(Tf, dut.is_busy, "dut.is_busy");
        sc_trace(Tf, dut.result, "dut.result");
    }

    ~shift_add_multiplier_tb() {
        sc_close_vcd_trace_file(Tf);
    }
};

int sc_main(int argc, char** argv) {
    shift_add_multiplier_tb tb("tb");
    sc_start();
    return 0;
}
