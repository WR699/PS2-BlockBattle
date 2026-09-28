EE_BIN = TOTRUS.ELF
EE_OBJS = src/main.o src/input.o src/screen.o

EE_INCS += -Iinclude
EE_INCS += -I$(GSKIT)/include
EE_INCS += -I$(GSKIT)/ee/dma/include
EE_INCS += -I$(GSKIT)/ee/gs/include

EE_LDFLAGS += -L$(GSKIT)/lib
EE_LIBS = -lgskit -ldmakit -ldraw -lmath3d -lpad -lm

all: $(EE_BIN)

clean:
	rm -f $(EE_BIN) $(EE_OBJS) TOTRUS.map

include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.eeglobal