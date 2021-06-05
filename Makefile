CXX=g++

INCS=-I./include

LIBS=-L./lib -lglfw3 -lglew32s -lopengl32 -lgdi32

SRC_DIR=src

all: epicycles

epicycles: epicycles.o
	$(CXX) $^ $(LIBS) -o $@

epicycles.o: $(SRC_DIR)/epicycles.cpp
	$(CXX) -c $(INCS) $^ -o $@

.PHONY: clean
clean:
	rm epicycles *.o