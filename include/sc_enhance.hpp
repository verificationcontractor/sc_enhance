#ifndef __SC_PROCESS_UTILS_HPP__
#define __SC_PROCESS_UTILS_HPP__

#define SC_INCLUDE_DYNAMIC_PROCESSES
#include <systemc>
#include <functional>
#include <vector>

#ifndef SCE_EXCLUDE_MP
#include "sc_method_ports.hpp"
#endif

// *******************
// * SC_METHOD wrapper
// *******************
template <class T>
struct sc_method_wrapper {

  typedef void (T::*sc_method_fp)();

  std::function<void(void)> run = [](){};

  sc_method_wrapper() {}

  sc_method_wrapper(
    const char* name
  , T* this_module 
  , sc_method_fp fp
  , std::function<void(void)> l_connect
  , std::function<void(void)> l_run
    )
  {
    this->run = l_run;

    ::sc_core::sc_process_handle handle =
        sc_core::sc_get_curr_simcontext()->create_method_process(
            name, false, // SC_MAKE_FUNC_PTR(host_tag, func),
            static_cast<sc_core::SC_ENTRY_FUNC>(fp),
            //reinterpret_cast<sc_core::SC_ENTRY_FUNC>(&sc_method_wrapper<T>::run_method),
            this_module, 0);
    this_module->sensitive     << handle;
    this_module->sensitive_pos << handle;
    this_module->sensitive_neg << handle;
    l_connect();
  }

};

#define SC_METHOD_DECLARE(name) \
 void name() { \
   name ## _method_wrapper.run(); \
 } \
sc_method_wrapper<SC_CURRENT_USER_MODULE> name ## _method_wrapper { \
    #name, \
    this, \
    &SC_CURRENT_USER_MODULE::name, \
    [this]() {

#define SC_METHOD_BEGIN \
    }, \
    [this]() {

#define SC_METHOD_END \
    } };

#define SC_METHOD_DECLARE_EMPTY(name) \
sc_method_wrapper<SC_CURRENT_USER_MODULE> name ## _method_wrapper; \
void name() { \
  name ## _method_wrapper.run(); \
}

#define SC_METHOD_IMPLEMENT(name) \
name ## _method_wrapper.run = [this]()

#define SC_METHOD_DECL(name, lambda) \
sc_method_wrapper<SC_CURRENT_USER_MODULE> name ## _method_wrapper { \
    #name, \
    this, \
    &SC_CURRENT_USER_MODULE::name, \
    (lambda), nullptr}; \
    void name()

#define SC_METHOD_DECL_EMPTY(name, lambda) SC_METHOD_DECL(name,lambda);

// ********************
// * SC_THREAD wrapper
// ********************
template <class T>
struct sc_thread_wrapper {

  typedef void (T::*sc_method_fp)();

  std::function<void(void)> run = [](){};

  sc_thread_wrapper() {}

  sc_thread_wrapper(
    const char* name
  , T* this_module 
  , sc_method_fp fp
  , std::function<void(void)> l_connect
  , std::function<void(void)> l_run
    )
  {
    this->run = l_run;

    ::sc_core::sc_process_handle handle =
        sc_core::sc_get_curr_simcontext()->create_thread_process(
            name, false, 
            static_cast<sc_core::SC_ENTRY_FUNC>(fp),
            this_module, 0);
    this_module->sensitive     << handle;
    this_module->sensitive_pos << handle;
    this_module->sensitive_neg << handle;
    l_connect();
  }

};

#define SC_THREAD_DECLARE(name) \
 void name() { \
   name ## _thread_wrapper.run(); \
 } \
sc_thread_wrapper<SC_CURRENT_USER_MODULE> name ## _thread_wrapper { \
    #name, \
    this, \
    &SC_CURRENT_USER_MODULE::name, \
    [this]() {

#define SC_THREAD_BEGIN \
    }, \
    [this]() {

#define SC_THREAD_END \
  } };

#define SC_THREAD_DECLARE_EMPTY(name) \
sc_thread_wrapper<SC_CURRENT_USER_MODULE> name ## _thread_wrapper; \
void name() { \
  name ## _thread_wrapper.run(); \
}

#define SC_THREAD_IMPLEMENT(name) \
name ## _thread_wrapper.run = [this]()

#define SC_THREAD_DECL(name, lambda) \
sc_thread_wrapper<SC_CURRENT_USER_MODULE> name ## _thread_wrapper { \
    #name, \
    this, \
    &SC_CURRENT_USER_MODULE::name, \
    (lambda), nullptr}; \
    void name()

#define SC_THREAD_DECL_EMPTY(name, lambda) SC_THREAD_DECL(name,lambda);

// ********************
// * SC_CTHREAD wrapper
// ********************
template <class T>
struct sc_cthread_wrapper {

  typedef void (T::*sc_method_fp)();

  std::function<void(void)> run = [](){};

  ::sc_core::sc_process_handle handle;

  sc_cthread_wrapper() {}

  sc_cthread_wrapper(
    const char* name
  , T* this_module 
  , sc_method_fp fp
  , std::function<void(void)> l_connect
  , std::function<void(void)> l_run
    )
  {
    this->run = l_run;

    handle =
        sc_core::sc_get_curr_simcontext()->create_cthread_process(
            name, false, 
            static_cast<sc_core::SC_ENTRY_FUNC>(fp),
            this_module, 0);
    l_connect();
  }

};

#define SC_CTHREAD_DECLARE(name, edge) \
 void name() { \
   name ## _cthread_wrapper.run(); \
 } \
sc_cthread_wrapper<SC_CURRENT_USER_MODULE> name ## _cthread_wrapper { \
    #name, \
    this, \
    &SC_CURRENT_USER_MODULE::name, \
    [this]() { \
    this->sensitive.operator() ( name ## _cthread_wrapper.handle, (edge) );

#define SC_CTHREAD_BEGIN \
    }, \
    [this]() {

#define SC_CTHREAD_END \
  } };

#define SC_CTHREAD_DECLARE_EMPTY(name) \
sc_cthread_wrapper<SC_CURRENT_USER_MODULE> name ## _cthread_wrapper; \
void name() { \
  name ## _cthread_wrapper.run(); \
}

#define SC_CTHREAD_IMPLEMENT(name) \
name ## _cthread_wrapper.run = [this]()

#define SC_CTHREAD_DECL(name, edge) \
sc_cthread_wrapper<SC_CURRENT_USER_MODULE> name ## _cthread_wrapper { \
    #name, \
    this, \
    &SC_CURRENT_USER_MODULE::name, \
    [this]() { \
    this->sensitive.operator() ( name ## _cthread_wrapper.handle, (edge) ); \
}, nullptr }; \
void name()

#define SC_CTHREAD_DECL_EMPTY(name, edge) SC_CTHREAD_DECL(name, edge);

// Declaration utilities, use together with SC_HAS_PROCESS
#define SC_DECLARE_UTILS(name) \
friend class sc_method_wrapper<name>; \
friend class sc_thread_wrapper<name>

// Process utilities, use instead of SC_HAS_PROCESS
#define SC_PROCESS_UTILS(name) \
  SC_HAS_PROCESS(name); \
  friend class sc_method_wrapper<name>; \
  friend class sc_thread_wrapper<name>

// Macros for shorter signal and instance declarations
#define sc_in_decl(T, name) ::sc_core::sc_in<T > name { #name }
#define sc_out_decl(T, name) ::sc_core::sc_out<T > name { #name }
#define sc_signal_decl(T, name) ::sc_core::sc_signal<T > name { #name }
#define sc_instance(T, name, ...) T name { #name , ## __VA_ARGS__ }

// Module constructor
#define SC_CONS(module, ...) module(::sc_core::sc_module_name name, ## __VA_ARGS__): ::sc_core::sc_module(name)
#define SC_CONS_EMPTY(module, ...) module(::sc_core::sc_module_name name, ## __VA_ARGS__)
#define SC_CONS_IMPLEMENT(module, ...) module::module(::sc_core::sc_module_name name, ## __VA_ARGS__): ::sc_core::sc_module(name)

// Port connections
struct lambda_wrapper {
  lambda_wrapper(std::function<void(void)> body) {
    body();
  }
};
#define COMBINE1(X,Y) X##Y  // helper macro
#define COMBINE(X,Y) COMBINE1(X,Y)
#define CONNECT_PORTS_BEGIN lambda_wrapper COMBINE(lw, __LINE__) { [this]() {
#define CONNECT_PORTS_END } };

// Repeat n times
#define sc_repeat(n) for(int COMBINE(i, __LINE__) = 0; COMBINE(i, __LINE__) < n; COMBINE(i, __LINE__)++)

// Better SC_FORK
#include "sc_thread_process.h"
namespace sc_core {

struct sc_thread_id_gen {
  static int id;
  static std::string get_id() {
    return std::to_string(id++);
  }
};
int sc_thread_id_gen::id = 0;

class sc_join_any : ::sc_core::sc_join {
  public:
  bool is_finished = false;

  void wait_clocked() {
    do {
      ::sc_core::wait();
    } while (!is_finished);
  }

  void signal(::sc_core::sc_thread_handle thread_p, int type)
  {
    switch ( type )
    {
      case ::sc_core::sc_process_monitor::spm_exit:
        thread_p->remove_monitor(this);
        m_threads_n--;
        is_finished = true;
        m_join_event.notify();
        break;
    }
  }
  
  void add_process( sc_process_handle process_h ) { 
    ::sc_core::sc_join::add_process(process_h); 
  }

  void wait() {
    ::sc_core::sc_join::wait();
  }

};

sc_core::sc_spawn_options default_spawn_opts;

typedef std::vector<std::function<void(void)> > sc_fork_list;

}

#undef SC_FORK
#undef SC_JOIN
#undef SC_CJOIN
#define SC_FORK \
{ \
  std::function<void(void)> forkees[] = {

#define SC_CFORK(edge) \
{ \
  sc_core::sc_spawn_options opts; \
  opts.set_sensitivity(&(edge)); \
  std::function<void(void)> forkees[] = {

#define SC_JOIN  }; \
  sc_core::sc_join           join; \
  sc_core::sc_spawn_options opts;\
  std::string s = ""; \
  for ( unsigned int i = 0; i < sizeof(forkees)/sizeof(std::function<void(void)>); i++ ) {\
          s = std::string(basename()) + "_thread_" + sc_thread_id_gen::get_id() + "_" + std::to_string(__LINE__) + "_" + std::to_string(i); \
          sc_process_handle handle = sc_spawn(forkees[i], s.c_str(), &opts); \
          join.add_process(handle); \
  } \
  join.wait(); \
}

#define SC_FORK_JOIN(LIST) \
{\
  sc_core::sc_join           join; \
  sc_core::sc_spawn_options opts;\
  std::string s = ""; \
  for ( unsigned int i = 0; i < (LIST).size(); i++ ) {\
          s = std::string(basename()) + "_thread_" + sc_thread_id_gen::get_id() + "_" + std::to_string(__LINE__) + "_" + std::to_string(i); \
          sc_process_handle handle = sc_spawn((LIST)[i], s.c_str(), &opts); \
          join.add_process(handle); \
  } \
  join.wait(); \
}

#define SC_JOIN_ANY  }; \
  sc_core::sc_join_any           join_any; \
  sc_core::sc_spawn_options opts;\
  std::string s = ""; \
  for ( unsigned int i = 0; i < sizeof(forkees)/sizeof(std::function<void(void)>); i++ ) {\
          s = std::string(basename()) + "_thread_" + sc_thread_id_gen::get_id() + "_" + std::to_string(__LINE__) + "_" + std::to_string(i); \
          sc_process_handle handle = sc_spawn(forkees[i], s.c_str(), &opts); \
          join_any.add_process(handle); \
  } \
  join_any.wait(); \
}

#define SC_FORK_JOIN_ANY(LIST) \
{\
  sc_core::sc_join_any           join_any; \
  sc_core::sc_spawn_options opts;\
  std::string s = ""; \
  for ( unsigned int i = 0; i < (LIST).size(); i++ ) {\
          s = std::string(basename()) + "_thread_" + sc_thread_id_gen::get_id() + "_" + std::to_string(__LINE__) + "_" + std::to_string(i); \
          sc_process_handle handle = sc_spawn((LIST)[i], s.c_str(), &opts); \
          join_any.add_process(handle); \
  } \
  join_any.wait(); \
}

#define SC_JOIN_FIRST  }; \
  sc_core::sc_join_any           join_any; \
  sc_core::sc_spawn_options opts;\
  std::string s = ""; \
  std::vector<sc_process_handle> procs; \
  sc_process_handle handle; \
  for ( unsigned int i = 0; i < sizeof(forkees)/sizeof(std::function<void(void)>); i++ ) {\
          s = std::string(basename()) + "_thread_" + sc_thread_id_gen::get_id() + "_" + std::to_string(__LINE__) + "_" + std::to_string(i); \
          handle = sc_spawn(forkees[i], s.c_str(), &opts); \
          join_any.add_process(handle); \
          procs.push_back(handle); \
  } \
  join_any.wait(); \
  for ( unsigned int i = 0; i < sizeof(forkees)/sizeof(std::function<void(void)>); i++ ) {\
          procs[i].kill(SC_INCLUDE_DESCENDANTS); \
  }\
}

#define SC_FORK_JOIN_FIRST(LIST) \
{\
  sc_core::sc_join_any           join_any; \
  sc_core::sc_spawn_options opts;\
  std::string s = ""; \
  std::vector<sc_process_handle> procs; \
  sc_process_handle handle; \
  for ( unsigned int i = 0; i < (LIST).size(); i++ ) {\
          s = std::string(basename()) + "_thread_" + sc_thread_id_gen::get_id() + "_" + std::to_string(__LINE__) + "_" + std::to_string(i); \
          handle = sc_spawn((LIST)[i], s.c_str(), &opts); \
          join_any.add_process(handle); \
          procs.push_back(handle); \
  } \
  join_any.wait(); \
  for ( unsigned int i = 0; i < procs.size(); i++ ) {\
          procs[i].kill(SC_INCLUDE_DESCENDANTS); \
  }\
}

#define SC_JOIN_NONE  }; \
  sc_core::sc_spawn_options opts;\
  std::string s = ""; \
  for ( unsigned int i = 0; i < sizeof(forkees)/sizeof(std::function<void(void)>); i++ ) {\
          s = std::string(basename()) + "_thread_" + sc_thread_id_gen::get_id() + "_" + std::to_string(__LINE__) + "_" + std::to_string(i); \
          sc_process_handle handle = sc_spawn(forkees[i], s.c_str(), &opts); \
  } \
}

#define SC_FORK_JOIN_NONE(LIST) \
{\
  sc_core::sc_spawn_options opts;\
  std::string s = ""; \
  for ( unsigned int i = 0; i < (LIST).size(); i++ ) {\
          s = std::string(basename()) + "_thread_" + sc_thread_id_gen::get_id() + "_" + std::to_string(__LINE__) + "_" + std::to_string(i); \
          sc_process_handle handle = sc_spawn((LIST)[i], s.c_str(), &opts); \
  } \
}


#define SC_CJOIN  }; \
  sc_core::sc_join           join; \
  std::string s = ""; \
  for ( unsigned int i = 0; i < sizeof(forkees)/sizeof(std::function<void(void)>); i++ ) {\
          s = std::string(basename()) + "_cthread_" + sc_thread_id_gen::get_id() + "_" + std::to_string(__LINE__) + "_" + std::to_string(i); \
          sc_process_handle handle = sc_spawn(forkees[i], s.c_str(), &opts); \
          join.add_process(handle); \
  } \
  join.wait_clocked(); \
}

#define SC_CFORK_CJOIN(LIST, edge) \
{\
  sc_core::sc_join           join; \
  std::string s = ""; \
  sc_core::sc_spawn_options opts; \
  opts.set_sensitivity(&(edge)); \
  for ( unsigned int i = 0; i < (LIST).size(); i++ ) {\
          s = std::string(basename()) + "_cthread_" + sc_thread_id_gen::get_id() + "_" + std::to_string(__LINE__) + "_" + std::to_string(i); \
          sc_process_handle handle = sc_spawn((LIST)[i], s.c_str(), &opts); \
          join.add_process(handle); \
  } \
  join.wait_clocked(); \
}

#define SC_CJOIN_ANY  }; \
  sc_core::sc_join_any           join_any; \
  std::string s = ""; \
  for ( unsigned int i = 0; i < sizeof(forkees)/sizeof(std::function<void(void)>); i++ ) {\
          s = std::string(basename()) + "_cthread_" + sc_thread_id_gen::get_id() + "_" + std::to_string(__LINE__) + "_" + std::to_string(i); \
          sc_process_handle handle = sc_spawn(forkees[i], s.c_str(), &opts); \
          join_any.add_process(handle); \
  } \
  join_any.wait_clocked(); \
}

#define SC_CFORK_CJOIN_ANY(LIST, edge) \
{\
  sc_core::sc_join_any           join_any; \
  std::string s = ""; \
  sc_core::sc_spawn_options opts; \
  opts.set_sensitivity(&(edge)); \
  for ( unsigned int i = 0; i < (LIST).size(); i++ ) {\
          s = std::string(basename()) + "_cthread_" + sc_thread_id_gen::get_id() + "_" + std::to_string(__LINE__) + "_" + std::to_string(i); \
          sc_process_handle handle = sc_spawn((LIST)[i], s.c_str(), &opts); \
          join_any.add_process(handle); \
  } \
  join_any.wait_clocked(); \
}

#define SC_CJOIN_FIRST  }; \
  sc_core::sc_join_any           join_any; \
  std::string s = ""; \
  sc_process_handle handle; \
  std::vector<sc_process_handle> procs; \
  for ( unsigned int i = 0; i < sizeof(forkees)/sizeof(std::function<void(void)>); i++ ) {\
          s = std::string(basename()) + "_cthread_" + sc_thread_id_gen::get_id() + "_" + std::to_string(__LINE__) + "_" + std::to_string(i); \
          handle = sc_spawn(forkees[i], s.c_str(), &opts); \
          join_any.add_process(handle); \
          procs.push_back(handle); \
  } \
  join_any.wait_clocked(); \
  for ( unsigned int i = 0; i < sizeof(forkees)/sizeof(std::function<void(void)>); i++ ) {\
          procs[i].kill(SC_INCLUDE_DESCENDANTS); \
  }\
}

#define SC_CFORK_CJOIN_FIRST(LIST, edge) \
{\
  sc_core::sc_join_any           join_any; \
  std::string s = ""; \
  sc_core::sc_spawn_options opts; \
  opts.set_sensitivity(&(edge)); \
  sc_process_handle handle; \
  std::vector<sc_process_handle> procs; \
  for ( unsigned int i = 0; i < (LIST).size(); i++ ) {\
          s = std::string(basename()) + "_cthread_" + sc_thread_id_gen::get_id() + "_" + std::to_string(__LINE__) + "_" + std::to_string(i); \
          handle = sc_spawn((LIST)[i], s.c_str(), &opts); \
          join_any.add_process(handle); \
          procs.push_back(handle); \
  } \
  join_any.wait_clocked(); \
  for ( unsigned int i = 0; i < procs.size(); i++ ) {\
          procs[i].kill(SC_INCLUDE_DESCENDANTS); \
  }\
}


#define SC_CJOIN_NONE  }; \
  std::string s = ""; \
  for ( unsigned int i = 0; i < sizeof(forkees)/sizeof(std::function<void(void)>); i++ ) {\
          s = std::string(basename()) + "_cthread_" + sc_thread_id_gen::get_id() + "_" + std::to_string(__LINE__) + "_" + std::to_string(i); \
          sc_process_handle handle = sc_spawn(forkees[i], s.c_str(), &opts); \
  } \
}

#define SC_CFORK_CJOIN_NONE(LIST, edge) \
{\
  std::string s = ""; \
  sc_core::sc_spawn_options opts; \
  opts.set_sensitivity(&(edge)); \
  for ( unsigned int i = 0; i < (LIST).size(); i++ ) {\
          s = std::string(basename()) + "_cthread_" + sc_thread_id_gen::get_id() + "_" + std::to_string(__LINE__) + "_" + std::to_string(i); \
          sc_process_handle handle = sc_spawn((LIST)[i], s.c_str(), &opts); \
  } \
}

#define SC_SPAWN(lambda) sc_spawn(lambda, (std::string(basename()) + "_cthread_" + sc_thread_id_gen::get_id() + "_" + std::to_string(__LINE__)).c_str(), &sc_core::default_spawn_opts)

#endif // __SC_PROCESS_UTILS_HPP__
