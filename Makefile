CXX = g++
CXXFLAGS = -std=c++17 
INCLUDES = -I include
LIBS = -lsystemc

clean:
	rm -f sim
	rm -f traces.vcd
	find tests/ -type f | grep -v golden | grep -v cpp | xargs rm -f

simple_fork:
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBS) -o sim examples/simple_fork.cpp

shift_add_multiplier:
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBS) -o sim examples/shift_add_multiplier.cpp

test_connect_ports:
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBS) -o tests/test_connect_ports tests/test_connect_ports.cpp

test_constructors:
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBS) -o tests/test_constructors tests/test_constructors.cpp

test_sc_fork:
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBS) -o tests/test_sc_fork tests/test_sc_fork.cpp

test_sc_process_wrapper:
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBS) -o tests/test_sc_process_wrapper tests/test_sc_process_wrapper.cpp

test_sc_process_wrapper2:
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBS) -o tests/test_sc_process_wrapper2 tests/test_sc_process_wrapper2.cpp

test_sc_repeat:
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBS) -o tests/test_sc_repeat tests/test_sc_repeat.cpp

test_sc_loopfork_join:
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBS) -o tests/test_sc_loopfork_join tests/test_sc_loopfork_join.cpp

test_sc_method_ports:
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBS) -o tests/test_sc_method_ports tests/test_sc_method_ports.cpp

build_tests: test_connect_ports test_constructors test_sc_fork test_sc_process_wrapper test_sc_process_wrapper2 test_sc_repeat test_sc_loopfork_join test_sc_method_ports

run_tests:
	ruby run_tests.rb
