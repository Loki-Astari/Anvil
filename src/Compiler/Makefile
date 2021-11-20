THORSANVIL_ROOT             = $(realpath ../../)

LDFLAGS						+= "-L/usr/local/opt/flex/lib"
CPPFLAGS					+= "-I/usr/local/opt/flex/include"
LEX							= /usr/local/opt/flex/bin/flex

TARGET                      = Compiler.slib
Ice_LDLIBS					= -lfl

%/ice.lex.o:				FILE_WARNING_FLAGS += -Wno-sign-compare
%/Action.o:					FILE_WARNING_FLAGS += -Wno-unknown-pragmas
%/Semantic.o:				FILE_WARNING_FLAGS += -Wno-unknown-pragmas
FILE_WARNING_FLAGS			= -Wno-unused-private-field

include $(THORSANVIL_ROOT)/build/tools/Makefile
