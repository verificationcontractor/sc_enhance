#include <systemc>
#include <vector>
#include <functional>

using namespace sc_core;
using namespace sc_dt;

template <class T>
struct sc_method_wrapper {

  typedef void (T::*sc_method_fp)();

  sc_method_wrapper(
    const char* name
  , T* this_module 
  , sc_method_fp mfp
  , std::vector<sc_event_finder*> levents
    )
  {
    //T* this_module ;
    //sc_method_fp mfp;

    ::sc_core::sc_process_handle handle =
        sc_core::sc_get_curr_simcontext()->create_method_process(
            name, false, // SC_MAKE_FUNC_PTR(host_tag, func),
            static_cast<sc_core::SC_ENTRY_FUNC>(mfp),
            this_module, 0);
    this_module->sensitive     << handle;
    this_module->sensitive_pos << handle;
    this_module->sensitive_neg << handle;
    std::cout << "Adding sensitive events..." << std::endl;
    //this_module->get_sensitive() << *ev;
    for(auto ev : levents)
      this_module->get_sensitive() << *ev;
    std::cout << "Done adding sensitive events." << std::endl;
  }

};

#define SC_METHOD_DECL(name, events, body) \
  void name() body \
  sc_method_wrapper<SC_CURRENT_USER_MODULE> name ## _method_wrapper { \
  #name, \
  this, \
  &SC_CURRENT_USER_MODULE::name, \
  std::vector<sc_event_finder*> events };


struct test_module: public sc_module {

  SC_HAS_PROCESS(test_module);

  friend class sc_method_wrapper<test_module>;
  sc_sensitive& get_sensitive() { return sensitive; }
  sc_sensitive_pos& get_sensitive_pos() { return sensitive_pos; }
  sc_sensitive_neg& get_sensitive_neg() { return sensitive_neg; }

  sc_in<bool> clk {"clk"};

  int count = 0;

   SC_METHOD_DECL(count_logic, ({&clk.pos(), &clk.neg()}) , 
   {
     int x = (1,2);
     std::cout << "count, = " << count << std::endl;
     count++;
     if(count == 10) sc_stop();
   })
 
  test_module(sc_module_name name):sc_module(name) {
  }

};

int sc_main(int argc, char** argv) {
  sc_clock clk("clk", 1, SC_NS, 0.5);
  test_module test("test");
  test.clk(clk);
  sc_start();
  return 0;
}
