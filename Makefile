CXX=g++

INCS=-I./include -I./src

LIBS=-L./lib -lglfw3 -lglew32s -lopengl32 -lgdi32

SRC_DIR=src

all: epicycles

epicycles: $(SRC_DIR)/epicycles.cpp
	$(CXX) $^ $(INCS) $(LIBS) -o $@

.PHONY: clean
clean:
	rm epicycles *.o