CXX=g++
CXXFLAGS += -std=c++11
FLAGS_OPENGL = -lGL -lGLU -lglut

abm = agent_based_model
ns = numerical_simulation

all: run clean
.PHONY: all

run: agent numerical
	@./agent && ./numerical

agent: $(abm)/main.cpp
	@$(CXX) $(CXXFLAGS) -o agent $(abm)/main.cpp $(FLAGS_OPENGL)

numerical: $(ns)/lotka_volterra.cpp
	@$(CXX) $(CXXFLAGS) -o numerical $(ns)/lotka_volterra.cpp 

clean:
	@rm agent numerical
