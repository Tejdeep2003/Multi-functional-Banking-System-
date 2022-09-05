BUILD_DIR   = ./build
BIN_DIR     = ./bin
INCLUDE_DIR = ./include
SRC_DIR = ./src

CC          = gcc -I $(INCLUDE_DIR)
LD          = gcc -I $(INCLUDE_DIR)

POPULATE_PROGRAM = populate
CLIENT_PROGRAM = client
SERVER_PROGRAM = server


SOURCES = $(wildcard $(SRC_DIR)/*.c)

CFLAG = -pthread

run: $(BIN_DIR)/$(POPULATE_PROGRAM) $(BIN_DIR)/$(CLIENT_PROGRAM) $(BIN_DIR)/$(SERVER_PROGRAM)

populate: $(BIN_DIR)/$(POPULATE_PROGRAM)
	./$(BIN_DIR)/$(POPULATE_PROGRAM)

client: $(BIN_DIR)/$(CLIENT_PROGRAM)
	./$(BIN_DIR)/$(CLIENT_PROGRAM)

server: $(BIN_DIR)/$(SERVER_PROGRAM)
	./$(BIN_DIR)/$(SERVER_PROGRAM)

$(BIN_DIR)/$(POPULATE_PROGRAM): $(BUILD_DIR)/populate.o 
	$(LD) $(BUILD_DIR)/populate.o -o $(BIN_DIR)/$(POPULATE_PROGRAM)

$(BIN_DIR)/$(CLIENT_PROGRAM): $(BUILD_DIR)/client.o 
	$(LD) $(BUILD_DIR)/client.o -o $(BIN_DIR)/$(CLIENT_PROGRAM)

$(BIN_DIR)/$(SERVER_PROGRAM): $(BUILD_DIR)/server.o $(BUILD_DIR)/db.o
	$(LD) $(CFLAG) $(BUILD_DIR)/server.o $(BUILD_DIR)/db.o -o $(BIN_DIR)/$(SERVER_PROGRAM)


$(BUILD_DIR)/server.o: $(SRC_DIR)/server.c $(INCLUDE_DIR)/ds.h  $(INCLUDE_DIR)/db.h $(INCLUDE_DIR)/server.h
	$(CC) -c $(SRC_DIR)/server.c -o $(BUILD_DIR)/server.o

$(BUILD_DIR)/client.o: $(SRC_DIR)/client.c $(INCLUDE_DIR)/ds.h   $(INCLUDE_DIR)/client.h
	$(CC) -c $(SRC_DIR)/client.c -o $(BUILD_DIR)/client.o

$(BUILD_DIR)/populate.o: $(SRC_DIR)/populate.c $(INCLUDE_DIR)/ds.h  $(INCLUDE_DIR)/db.h
	$(CC) -c $(SRC_DIR)/populate.c -o $(BUILD_DIR)/populate.o

$(BUILD_DIR)/db.o: $(INCLUDE_DIR)/db.h $(INCLUDE_DIR)/ds.h 
	$(CC) -c $(SRC_DIR)/db.c -o $(BUILD_DIR)/db.o


build: $(BIN_DIR)/$(PROG_NAME)

clean:
	rm -f $(BIN_DIR)/$(POPULATE_PROGRAM) $(BIN_DIR)/$(CLIENT_PROGRAM) $(BIN_DIR)/$(SERVER_PROGRAM) $(BUILD_DIR)/*.o db/*