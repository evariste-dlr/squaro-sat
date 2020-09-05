LIB_DIR = ./lib
SRC_DIR = ./src
BIN_DIR = ./bin
OBJ_DIR = ./obj

CFLAGS = -std=c99


all: $(BIN_DIR)/alea $(BIN_DIR)/plt_sat $(BIN_DIR)/print $(BIN_DIR)/red3sat $(BIN_DIR)/dpll


$(BIN_DIR)/alea: $(SRC_DIR)/alea.c $(OBJ_DIR)/plateau.o
	$(CC) $(CFLAGS) $^ -o $@

$(BIN_DIR)/plt_sat: $(SRC_DIR)/plt_sat.c $(OBJ_DIR)/plateau.o $(OBJ_DIR)/logique.o $(OBJ_DIR)/dimacs.o $(OBJ_DIR)/lire.o
	$(CC) $(CFLAGS) $^ -lm -o $@

$(BIN_DIR)/print: $(SRC_DIR)/print.c $(OBJ_DIR)/plateau.o $(OBJ_DIR)/lire.o
	$(CC) $(CFLAGS) $^ -o $@

$(BIN_DIR)/red3sat: $(SRC_DIR)/red3sat.c $(OBJ_DIR)/logique.o $(OBJ_DIR)/dimacs.o $(OBJ_DIR)/lire.o
	$(CC) $(CFLAGS) $^ -lm -o $@

$(BIN_DIR)/dpll: $(SRC_DIR)/dpll.c $(OBJ_DIR)/logique.o $(OBJ_DIR)/dimacs.o $(OBJ_DIR)/lire.o
	$(CC) $(CFLAGS) $^ -lm -o $@


$(OBJ_DIR)/%.o: $(LIB_DIR)/%.c $(LIB_DIR)/%.h $(OBJ_DIR)
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJ_DIR):
	mkdir $@

.PHONY: clean

clean:
	rm -f $(OBJ_DIR)/*.o
