BUILD_DIR = build
EE_BIN = $(BUILD_DIR)/BLOCKBATTLE.ELF
EE_OBJS = src/main.o src/input.o src/screen.o src/piece.o

EE_INCS += -Iinclude
EE_INCS += -I$(GSKIT)/include
EE_INCS += -I$(GSKIT)/ee/dma/include
EE_INCS += -I$(GSKIT)/ee/gs/include

EE_LDFLAGS += -L$(GSKIT)/lib
EE_LIBS = -lgskit -ldmakit -lpad -lm

all: $(EE_BIN)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(EE_BIN): | $(BUILD_DIR)

clean:
	rm -f $(EE_BIN) $(EE_OBJS) $(BUILD_DIR)/BLOCKBATTLE.map

run: $(EE_BIN)
	ps2client execee host:$(EE_BIN)

reset:
	ps2client reset

include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.eeglobal
