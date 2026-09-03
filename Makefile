CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude -O2
SRCS = src/value.cpp src/parser.cpp src/serializer.cpp src/schema.cpp src/patch.cpp
OBJS = $(SRCS:.cpp=.o)

all: bin/json_mini_app bin/run_tests

bin:
	mkdir -p bin

bin/json_mini_app: bin main.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) main.cpp $(OBJS) -o bin/json_mini_app

bin/run_tests: bin tests/test_main.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) tests/test_main.cpp $(OBJS) -o bin/run_tests

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: bin/run_tests
	./bin/run_tests

clean:
	rm -rf bin src/*.o
