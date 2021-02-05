clean:
	rm -f sim
	rm -f traces.vcd
	find tests/ -type f | grep -v golden | grep -v cpp | xargs rm -f

simple_fork:
	g++ -std=c++17 -o sim -I include -lsystemc examples/simple_fork.cpp

shift_add_multiplier:
	g++ -std=c++17 -o sim -I include -lsystemc examples/shift_add_multiplier.cpp

test_connect_ports:
	g++ -std=c++17 -o tests/test_connect_ports -I include -lsystemc tests/test_connect_ports.cpp

test_constructors:
	g++ -std=c++17 -o tests/test_constructors -I include -lsystemc tests/test_constructors.cpp

test_sc_fork:
	g++ -std=c++17 -o tests/test_sc_fork -I include -lsystemc tests/test_sc_fork.cpp

test_sc_process_wrapper:
	g++ -std=c++17 -o tests/test_sc_process_wrapper -I include -lsystemc tests/test_sc_process_wrapper.cpp

test_sc_repeat:
	g++ -std=c++17 -o tests/test_sc_repeat -I include -lsystemc tests/test_sc_repeat.cpp

test_sc_loopfork_join:
	g++ -std=c++17 -o tests/test_sc_loopfork_join -I include -lsystemc tests/test_sc_loopfork_join.cpp

build_tests: test_connect_ports test_constructors test_sc_fork test_sc_process_wrapper test_sc_repeat test_sc_loopfork_join

run_tests:
	ruby run_tests.rb
