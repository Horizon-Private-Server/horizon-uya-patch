EE_OBJS_DIR = obj/
EE_BIN_DIR = bin/

EE_ELF = $(EE_BIN_DIR)printf.elf
EE_BIN = $(EE_BIN_DIR)printf.bin
EE_OBJS = main.o
EE_LIBS = -ldl
ADDRESS = 000F0000

EE_OBJS := $(EE_OBJS:%=$(EE_OBJS_DIR)%)
EE_INCS := -I../../include
EE_LDFLAGS = -fno-builtin -nostdlib -nostartfiles -L. -L../../lib
EE_CFLAGS := -D_PROGRAM_ENTRY=0x$(ADDRESS) -DDEBUG

all: linker $(EE_OBJS_DIR) $(EE_BIN_DIR) $(EE_BIN)

$(EE_OBJS_DIR):
	mkdir -p $(EE_OBJS_DIR)

$(EE_BIN_DIR):
	mkdir -p $(EE_BIN_DIR)

$(EE_OBJS_DIR)%.o : $(EE_SRC_DIR)%.c
	$(EE_C_COMPILE) -c $< -o $@

$(EE_OBJS_DIR)%.o : $(EE_SRC_DIR)%.S
	$(EE_C_COMPILE) -c $< -o $@

linker:
	$(EE_CC) -E -x c $(EE_CFLAGS) ../linkfile.template | grep -v '^#' > linkfile

code:
	bin2code $(EE_BIN) $(ADDRESS) $(EE_BIN).raw raw
	cat ../hook.raw >> $(EE_BIN).raw
	bin2code $(EE_BIN) $(ADDRESS) $(EE_BIN).pnach pnach
	cat ../hook.pnach >> $(EE_BIN).pnach

clean:
	rm -rf linkfile
	rm -f -r $(EE_OBJS_DIR)
	rm -f -r $(EE_BIN_DIR)

include $(CURDIR)/../Makefile.pref
include $(CURDIR)/../Makefile.eeglobal
