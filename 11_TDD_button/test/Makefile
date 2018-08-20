CPP_FLAGS = -std=c++17 -I../../CMSIS -I.. -DTEST

all:
	g++ test_pin.cpp $(CPP_FLAGS)
	./a.out
	g++ test_cyclic_task.cpp $(CPP_FLAGS)
	./a.out
	g++ test_button.cpp $(CPP_FLAGS)
	./a.out

