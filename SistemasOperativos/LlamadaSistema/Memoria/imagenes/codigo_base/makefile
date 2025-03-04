# Makefile para compilar el programa de procesamiento de imágenes

CXX = g++
CXXFLAGS = -Wall -std=c++17 `pkg-config --cflags opencv4`
LDFLAGS = `pkg-config --libs opencv4`

SRC = main.cpp imagen.cpp stb_wrapper.cpp
OBJ = $(SRC:.cpp=.o)
EXEC = procesar_imagen

# Dependencias de STB
STB_FILES = stb_image.h stb_image_write.h

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(EXEC) $(OBJ) $(LDFLAGS) -lm

%.o: %.cpp $(STB_FILES)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)

run:
	./$(EXEC) imagen_prueba.jpg
