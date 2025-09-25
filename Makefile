
# C-kompilator (byt vid behov, t.ex. clang)
CC := gcc
# Katalog där källfilerna finns.
SRC_DIR := src
# Katalog där objektfilerna ska placeras
BUILD_DIR := build
# Flaggor: standard, varningar, optimering + auto-dep för headers
CFLAGS := -std=c11 -Wall -Wextra -MMD -MP -Ilibs -Iincludes
# Länkarflaggor
LDFLAGS := -flto -Wl,--gc-sections
# Bibliotek att länka mot
LIBS := -lcurl
# Hittar alla .c filer rekursivt i katalogerna src och libs
SRC := $(shell find -L $(SRC_DIR) libs -type f -name '*.c')
# Mappa varje .c till motsvarande .o i BUILD_DIR
OBJ := $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRC))
# Tillhörande .d-filer (dependency-filer skapade av -MMD)
DEP := $(OBJ:.o=.d)
# Namnet på den körbara filen
BIN := $(BUILD_DIR)/main

# Standardmål: bygg binären
all: $(BIN)
	@echo "Build complete."

# Länksteg: binären beror på alla objektfiler
$(BIN): $(OBJ)
	@$(CC) $(LDFLAGS) $(OBJ) -o $@ $(LIBS)

# Mönsterregel: bygger en .o från motsvarande .c
$(BUILD_DIR)/%.o: %.c
	@echo "Compiling $<..."
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Hjälpmål: kör programmet om det är byggt
run: $(BIN)
	./$(BIN)

# Hjälpmål: städa bort genererade filer
clean:
	@rm -rf $(BUILD_DIR) $(BIN)

# Hjälpmål: skriv ut variabler för felsökning
print:
	@echo "Källfiler: $(SRC)"
	@echo "Objektfiler: $(OBJ)"
	@echo "Dependency-filer: $(DEP)"

# Inkludera header-beroenden (prefix '-' = ignorera om de inte finns ännu)
-include $(DEP)

# Dessa mål är inte riktiga filer; kör alltid när de anropas
.PHONY: all run clean

