# Nombre del ejecutable
TARGET = tlb_demo

# Compilador
CXX = g++
CXXFLAGS = -std=c++11 -O2

all: $(TARGET)

$(TARGET): tlb_demo.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) tlb_demo.cpp

clean:
	rm -f $(TARGET)
