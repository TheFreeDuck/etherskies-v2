
# C-kompilator (byt vid behov, t.ex. clang)
# Detta är en enkel variabel definition
CC := gcc
# Katalog där källfilerna finns.
SRC_DIR := src
# Katalog där objektfilerna ska placeras
BUILD_DIR := build
# Flaggor: standard, varningar, optimering + auto-dep för headers
CFLAGS := -std=c11 -Wall -Wextra -MMD -MP -Ilib/jansson-2.13.1/src -Ilib/jansson-2.13.1/include
# Länkarflaggor
LDFLAGS := -flto -Wl,--gc-sections
# Bibliotek att länka mot
LIBS := -lcurl
# Hittar alla .c filer rekursivt i katalogen.
SRC := $(shell find -L $(SRC_DIR) -type f -name '*.c')
# Mappa varje .c till motsvarande .o i BUILD_DIR
OBJ := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC))
# Tillhörande .d-filer (dependency-filer skapade av -MMD)
DEP := $(OBJ:.o=.d)
# Namnet på den körbara filen
BIN := $(BUILD_DIR)/main


# -------------------------------------------------------------------
# >>> Mina tillägg för att bygga in Jansson direkt i projektet <<<
# Hitta alla Jansson .c filer i källkatalogen
JANSSON_SRC := $(shell find lib/jansson-2.13.1/src -type f -name '*.c')
# Skapa motsvarande .o-filer i build/jansson/
JANSSON_OBJ := $(patsubst lib/jansson-2.13.1/src/%.c,$(BUILD_DIR)/jansson/%.o,$(JANSSON_SRC))
# Lägg till Jansson-objekten i den globala listan av objektfiler
OBJ += $(JANSSON_OBJ)
# -------------------------------------------------------------------

# Standardmål: bygg binären
all: $(BIN)
	@echo "Build complete."

# Länksteg: binären beror på alla objektfiler
$(BIN): $(OBJ)
	@$(CC) $(LDFLAGS) $(OBJ) -o $@ $(LIBS)

# Mönsterregel: bygger en .o från motsvarande .c
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compiling $<..."
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# -------------------------------------------------------------------
# >>> Min extra mönsterregel för att bygga Jansson-källor <<<
$(BUILD_DIR)/jansson/%.o: lib/jansson-2.13.1/src/%.c
	@echo "Compiling Jansson $<..."
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@
# -------------------------------------------------------------------

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

