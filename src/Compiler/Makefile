THORSANVIL_ROOT             = $(realpath ../../)

LDFLAGS						+= "-L/usr/local/opt/flex/lib"
CPPFLAGS					+= "-I/usr/local/opt/flex/include"
LEX							= /usr/local/opt/flex/bin/flex

TARGET                      = Compiler.slib
Ice_LDLIBS					= -lfl

%/ice.lex.o:				FILE_WARNING_FLAGS += -Wno-sign-compare

include $(THORSANVIL_ROOT)/build/tools/Makefile
