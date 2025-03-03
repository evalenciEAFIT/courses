CXX = g++
CXXFLAGS = -std=c++17 `pkg-config --cflags opencv4`
LDFLAGS = `pkg-config --libs opencv4`
SRC = main.cpp Imagen.cpp VisorImagen.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = visor_imagen

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
