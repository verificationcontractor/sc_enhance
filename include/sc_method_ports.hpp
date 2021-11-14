#ifndef __LP_REGISTERED_H__
#define __LP_REGISTERED_H__

#include <iostream>
#include <functional>
#include <utility>
#include <string>
#include <exception>
#include <map>
#include <systemc>

//////////////////////////
// Port declaration macros
//////////////////////////
#define sc_out_method_port_declare(NAME, TYPE) sc_mp::sc_out_method_port<TYPE> NAME {#NAME, this} 
#define sc_in_method_port_declare(NAME, TYPE, LAMBDA) sc_mp::sc_in_method_port<TYPE> NAME {#NAME, this, (LAMBDA)}
#define sc_in_method_port_declare_empty(NAME, TYPE) sc_mp::sc_in_method_port<TYPE> NAME {#NAME, this}
#define sc_forward_method_port_declare(NAME, TYPE) lp::sc_forward_method_port<TYPE> NAME {#NAME, this}

//#define declare_global_out_port(NAME, TYPE) lp::sc_out_method_port<TYPE> NAME {#NAME, NULL} 
//#define declare_global_in_port(NAME, TYPE, LAMBDA) lp::in_port<TYPE> NAME {#NAME, NULL, (LAMBDA)}
//#define declare_global_in_port_empty(NAME, TYPE) lp::in_port<TYPE> NAME {#NAME, NULL}
//#define declare_global_forward_port(NAME, TYPE) lp::sc_forward_method_port<TYPE> NAME {#NAME, NULL}

namespace sc_mp {
///////////////
// Simple ports
///////////////

// sc_simple_in_method_port - receives a method call and executes it
template<class T> 
struct sc_simple_in_method_port {
  std::function<T> call;

  sc_simple_in_method_port(std::function<T> call) {
    this->call = call;
  }

  sc_simple_in_method_port() {
    this->call = nullptr;
  }

};

// sc_simple_out_method_port - sends a method call
template<class T> 
struct sc_simple_out_method_port {
  private:
  sc_simple_in_method_port<T>* other = NULL;

  public:
  void connect(sc_simple_in_method_port<T>& other) {
    this->other = &other;
  }

  template<typename ...ArgTypes>
#if __cplusplus > 201103L
  auto operator()(ArgTypes&& ...args) {
#else
  auto operator()(ArgTypes&& ...args) -> decltype(this->other->call(std::forward<ArgTypes>(args)...)) {
#endif
    if(!is_connected()) throw std::runtime_error("Error from sc_simple_out_method_port<>::operator() - Port not connected.");
    if(other->call == nullptr) throw std::runtime_error("Error from sc_simple_out_method_port<>::operator() - Lambda of connected sc_simple_in_method_port is null.");
    return other->call(std::forward<ArgTypes>(args)...);
  }

  virtual bool is_connected() {
    return other != NULL;
  }

};

#if __cplusplus > 201103L
// sc_simple_forward_method_port - receives a method call and forwards it on a sc_simple_out_method_port
template<class T>
struct sc_simple_forward_method_port:public sc_simple_in_method_port<T> {
  sc_simple_out_method_port<T> forward_port_out {};

  sc_simple_forward_method_port():sc_simple_in_method_port<T>(nullptr) {
    this->call = [this] (auto ...args)->decltype(auto) {
       return forward_port_out(std::forward<decltype(args)>(args)...);
    };
  }

  void connect(sc_simple_in_method_port<T>& other) {
    forward_port_out.connect(other);
  }

};
#endif


//////////////////////////////////
// Named ports and components
//////////////////////////////////

class sc_in_method_port_base;
class sc_out_method_port_base;

struct sc_method_port_db {
  static std::map<sc_core::sc_module*, std::map<std::string, sc_in_method_port_base*> > in_method_ports;
  static std::map<sc_core::sc_module*, std::map<std::string, sc_out_method_port_base*> > out_method_ports;
  static std::map<sc_out_method_port_base*, sc_in_method_port_base*> method_port_connections;
  //static std::map<in_port_base*, out_port_base*> forwards; // TODO - how?
};

std::map<sc_core::sc_module*, std::map<std::string, sc_in_method_port_base*> > sc_method_port_db::in_method_ports {};
std::map<sc_core::sc_module*, std::map<std::string, sc_out_method_port_base*> > sc_method_port_db::out_method_ports {};
std::map<sc_out_method_port_base*, sc_in_method_port_base*> sc_method_port_db::method_port_connections {};


struct sc_method_port_base {
  std::string name; 
  virtual bool is_connected() = 0;
  sc_core::sc_module* parent = NULL;

  std::string get_name() { return name; }

  std::string get_full_name() {
    if(parent == NULL) return name;
    else return std::string(parent->name()) + "." + name;
  }

  sc_method_port_base(std::string name) {
    this->name = name;
  }
};

struct sc_in_method_port_base:sc_method_port_base {

  sc_in_method_port_base(std::string name):sc_method_port_base(name) {}

  virtual bool is_connected() {
    return conn_count > 0;
  }

  void inc_conn_count() { conn_count++; }
  void dec_conn_count() { conn_count--; }

  private:
  int conn_count = 0;
};

template<class T> 
struct sc_in_method_port:sc_in_method_port_base {
  std::function<T> call;

  sc_in_method_port(std::string name, sc_core::sc_module* parent=NULL, std::function<T> call=nullptr):sc_in_method_port_base(name) {
    this->call = call;
    this->parent = parent;
    sc_method_port_db::in_method_ports[parent][name] = this;
  }
  
};

struct sc_out_method_port_base:sc_method_port_base {
  sc_out_method_port_base(std::string name):sc_method_port_base(name) {}
};

template<class T>
struct sc_out_method_port:sc_out_method_port_base {

  private:
  sc_in_method_port<T>* other = NULL;
  
  public:
  sc_out_method_port(std::string name, sc_core::sc_module* parent):sc_out_method_port_base(name) {
    this->parent = parent;
    sc_method_port_db::out_method_ports[parent][name] = this;
  }

  sc_out_method_port(std::string name):sc_out_method_port(name, NULL) {}

  void connect(sc_in_method_port<T>& other) {
    // TODO : throw exception if other is null
    this->other = &other;
    this->other->inc_conn_count();
    sc_method_port_db::method_port_connections[this] = &other;
  }

  void disconnect() {
    // TODO : throw exception if other is null
    this->other.dec_conn_count();
    this->other = NULL;
  }

  template<typename ...ArgTypes>
#if __cplusplus > 201103L
  auto operator()(ArgTypes&& ...args) {
#else
  auto operator()(ArgTypes&& ...args) -> decltype(this->other->call(std::forward<ArgTypes>(args)...)) {
#endif
    if(!is_connected()) {
      throw std::runtime_error("Error from sc_out_method_port<>::operator(): Port " + get_full_name() + " not connected.");
    }
    if(other->call == nullptr) throw std::runtime_error("Error from sc_out_method_port<>::operator() - Port " + get_full_name() + " - Lambda of connected sc_in_method_port " + other->get_full_name() + " is null.");
    return other->call(std::forward<ArgTypes>(args)...);
  }

  virtual bool is_connected() {
    return other != NULL;
  }

};

#if __cplusplus > 201103L
template<class T>
struct sc_forward_method_port:public sc_simple_in_method_port<T> {
  sc_out_method_port<T> forward_port_out { "forward_port_out" };

  sc_forward_method_port(std::string name, sc_core::sc_module* parent):sc_in_method_port<T>(name,parent,nullptr) {
    this->call = [this] (auto ...args)->decltype(auto) {
      return forward_port_out(std::forward<decltype(args)>(args)...);
    };
  }

  sc_forward_method_port(std::string name):sc_forward_method_port(name, NULL) {}

  void connect(sc_simple_in_method_port<T>& other) {
    forward_port_out.connect(other);
    // TODO: lp_db::forwards[this] = &other;
  }

};
#endif

// TODO add thread safe ports

}

#endif // __LP_REGISTERED_H__