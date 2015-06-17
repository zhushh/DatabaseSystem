DIR_SRC =./src
DIR_OBJ = ./obj
DIR_BIN = ./bin

SRC = $(wildcard $(DIR_SRC)/*.cpp)
OBJ = $(patsubst %.cpp,$(DIR_OBJ)/%.o,$(notdir $(SRC)))

TARGET = app
BIN_TARGET = $(DIR_BIN)/$(TARGET)
CC = g++
CFLAGS = -g -Wall -I$(DIR_SRC)

$(BIN_TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@

$(DIR_OBJ)/%.o: $(DIR_SRC)/%.cpp $(DIR_SRC)/*.h
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY:clean
clean:
	rm -rf $(DIR_OBJ)/*.o $(BIN_TARGET) $(DIR_BIN)/*
