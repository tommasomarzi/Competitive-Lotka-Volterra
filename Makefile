CXX=g++
CXXFLAGS += -std=c++11
FLAGS_OPENGL = -lGL -lGLU -lglut

abm = agent_based_model
ns = numerical_simulation

all: run clean
.PHONY: all

run: agent numerical
	@./agent && ./numerical

agent: $(abm)/main_abm.cpp
	@$(CXX) $(CXXFLAGS) -o agent $(abm)/main_abm.cpp $(FLAGS_OPENGL)

numerical: $(ns)/main_ns.cpp
	@$(CXX) $(CXXFLAGS) -o numerical $(ns)/main_ns.cpp 

clean:
	@rm agent numerical
