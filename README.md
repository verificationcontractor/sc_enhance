# sc_enhance - simplifies SystemC using modern C++ features
sc_enhance is a header only library that can be included on top of SystemC in order to simplify the declaration and implementation of certain constructs from the SystemC language. To use sc_enhance you must compile the SystemC library with support for the latest C++ standard. This version of sc_enhance has been tested with C++17.

## Simplified process declarations
sc_enhance introduces macros SC_METHOD_DECLARE, SC_THREAD_DECLARE and SC_CTHREAD_DECLARE which allow processes to be declared and implemented in a compact manner in a single place. These declarations are similar to SystemVerilog's ```always``` and ```initial``` blocks. There is no need to register methods, threads and clocked threads in the constructor of the module class. Nor do you need to declare the sensitivities of each process in the constructor. All of this can be done where the process is declared and implemented making the code much easier to write and read as well. See an example below:

```cpp
#include <systemc>
#include "sc_enhance.hpp"
using namespace sc_core;
using namespace sc_dt;

SC_MODULE(demo) {
    SC_PROCESS_UTILS(demo); // some utilities needed to make our macros work

    sc_in<bool> clk {"clk"};
    sc_in<bool> reset {"reset"};
    int count = 0;

    // Declare method with sensitivities in the same place
    SC_METHOD_DECLARE(count_logic)
      sensitive << clk.pos();
    SC_METHOD_BEGIN
        if(reset.read() == 1)
            count = 0;
        else
            count++;
    SC_METHOD_END

    // Declare thread with sensitivities in the same place
    SC_THREAD_DECLARE(test_thread) 
      dont_initialize();
      sensitive << clk.pos();
    SC_THREAD_BEGIN
        while(1) {
            if(count % 2 == 0) std::cout << "Counter is even." << std::endl;
            wait(clk.posedge_event());
        }
    SC_THREAD_END

    // Declare clocked thread with sensitivities in the same place
    SC_CTHREAD_DECLARE(test_cthread, clk.pos())
    SC_CTHREAD_BEGIN
        while(1) {
            if(count % 2 == 1) std::cout << "Counter is odd." << std::endl;
            wait();
        }
    SC_CTHREAD_END

    SC_CONS(demo) {
        // no need to register processes in constructor
        // no need to declare lists of sensitivities in constructor
    }
};
```

If the code is separated into header and source files and the processes need to be implemented outside of the module class, you can declare them as emply inside the module class and implement them in the module constructor like this:

```cpp
SC_MODULE(demo) {
    SC_PROCESS_UTILS(demo)

    sc_in<bool> clk {"clk"};
    sc_in<reset> reset {"reset"};
    int count = 0;

    // Declare empty method 
    SC_METHOD_DECLARE_EMPTY(count_logic)

    // Declare empty thread 
    SC_THREAD_DECLARE_EMPTY(test_thread) 

    // Declare empty clocked thread
    SC_CTHREAD_DECLARE_EMPTY(test_cthread, clk.pos())

    SC_CONS_EMPTY(demo); // read below about this macro
};

SC_CONS_IMPLEMENT(demo) { // read below about this macro
  
  // Implement method here
  SC_METHOD(test_method);
    sensitive << clk.pos();
  SC_METHOD_IMPLEMENT(count_monitor_logic) {
        if(reset.read() == 1)
            count = 0;
        else
            count++;
  };
  
  // Implement thread here
  SC_THREAD(test_thread);
    dont_initialize();
    sensitive << clk.pos();
  SC_THREAD_IMPLEMENT(test_thread) {
        while(1) {
            if(count % 2 == 0) std::cout << "Counter is even." << std::endl;
            wait(clk.posedge_event());
        }
  };

  // Implement clocked thread here
  SC_CTHREAD(test_cthread, clk.pos());
  SC_CTHREAD_IMPLEMENT(test_cthread) {
        while(1) {
            if(count % 2 == 1) std::cout << "Counter is odd." << std::endl;
            wait();
        }
  };
}
```

Notice that in the second case we use the already existing macros SC_METHOD, SC_THREAD or SC_CTHREAD. There's no need to replace them. SystemC already has a lot of macros that can make our life easy. sc_enhance only attempts to complement them not to replace them completely.

## Simplified forks
SystemC already has some support for dynamically forked threads but it's pretty limited. sc_enhance allows you to declare the process methods inline using lambda expressions and adds more types of joins inspired from SystemVerilog's ```join_any```, ```join_none``` and Specman e's ```first_of``` (named here ```SC_JOIN_FIRST``` for consistency):
* SC_JOIN - waits for all threads to finish
* SC_JOIN_ANY - waits for one thread to finish then continues, the other forked threads keep on running
* SC_JOIN_FIRST - waits for one thread to finish then continues, the other forked threads are killed
* SC_JOIN_NONE - doesn't wait for any forked thread to finish, it only startes them and lets them run

Here's an example:
```cpp
SC_THREAD_DECLARE(fork_test_logic)
    dont_initialize();
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
    SC_JOIN_ANY // or SC_JOIN_FIRST, SC_JOIN_NONE, SC_JOIN
    wait(100, SC_NS);
    std::cout << "Done." << std::endl;
    sc_stop();
SC_THREAD_END
```

You can also fork clocked threads:
```cpp
SC_CTHREAD_DECLARE(fork_cthread_test_logic, clk)
SC_CTHREAD_BEGIN
    SC_CFORK(clk)
        [&](){
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
    sc_repeat(6) wait();
    std::cout << "CDone at " << sc_time_stamp() << std::endl;
    sc_stop();
SC_CTHREAD_END
```

## Simplified constructors
SystemC already has the SC_CTOR macro to shorten constructor declarations but it's pretty limited. It won't support constructors that take more than one argument and it also introduces a typedef which should be declared earlier. SC_EHANCE introduces SC_CONS which supports a variable number of arguments. If the constructor needs to be implemented outside of the module class, SC_EHANCE introduces SC_CONS_EMPY and SC_CONS_IMPLEMENT. See some examples below:

```cpp
SC_MODULE(demo) {
    SC_CONS(demo) { /* implement constructor here */}

    SC_CONS(demo, int x, int y) { /* implement constructor with arguments here */ }

    SC_CONS_EMPTY(demo, double d);
};

SC_CONS_IMPLEMENT(demo, double d) {
    // Implement constructor with argument outside of module class here
}
```

## Simplified port connections
SystemC module ports are usually connected in the constructor. With sc_enhance these connections can be declared right after the module instance declaration using the CONNECT_PORTS_BEGIN/CONNECT_PORTS_END macros.

```cpp
SC_MODULE(shift_add_multiplier_tb) {
    SC_PROCESS_UTILS(shift_add_multiplier_tb);

    sc_clock clk {"clk", sc_time(5, SC_NS), 0.5, SC_ZERO_TIME, true};
    sc_signal_decl(bool, reset); // read about signal declaration macros below
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

    SC_CONS(shift_add_multiplier_tb) { /* no need to connect signals here */ }
};
```

## Simplified signal and instance declarations
Signal and instance declarations can be redundant and verbose even if they are initialized with modern C++'s initializer lists. To make declarations easier to write and read as well, sc_enhance introduces the following macros:
* ```sc_in_decl(T, name)``` - expands to: ```sc_in<T> name {"name"}```
* ```sc_out_decl(T, name)``` - expands to: ```sc_out<T> name {"name"}```
* ```sc_signal_decl(T, name)``` - expands to: ```sc_signal<T> name {"name"}```
* ```sc_instance_decl(T, name, args)``` - expands to: ```T name {"name", args}```

## Simplified repetition loop
C++ ```for``` loops can also be verbose especially when all we want to do is repeat the same statement a number of times (e.g. wait for an event multiple times). This can be simplified by using the ```sc_repeat``` macro:
```cpp
sc_repeat(5) wait(ack.posedge_event());
```
## Conclusion
Modern C++ allows for more concise C++ code to be written and this can obviously be applied to SystemC. I'm sure such enhancements will be included in future versions of SystemC but until that is decided, sc_enhance can be used.
