EE_BIN = TOTRUS.ELF
EE_OBJS = src/main.o src/input.o

# Headers propios del proyecto
EE_INCS += -Iinclude

# gsKit / dmaKit
EE_INCS += -I$(GSKIT)/include
EE_INCS += -I$(GSKIT)/ee/dma/include
EE_INCS += -I$(GSKIT)/ee/gs/include

# Librerias de gsKit + base 3D de PS2SDK + mando
EE_LDFLAGS += -L$(GSKIT)/lib
EE_LIBS = -lgskit -ldmakit -ldraw -lmath3d -lpad -lm

all: $(EE_BIN)

clean:
	rm -f $(EE_BIN) $(EE_OBJS) TOTRUS.map

run: $(EE_BIN)
	ps2client execee host:$(EE_BIN)

reset:
	ps2client reset

include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.eeglobal
