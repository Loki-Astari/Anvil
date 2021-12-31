THORSANVIL_ROOT             = $(realpath ../../)

COVERAGE_REQUIRED			= 60

TEST_IGNORE					= ice.lex.cpp ice.lex.h ice.tab.cpp ice.tab.hpp
EXTRA_HEADERS				= ice.tab.hpp ice.lex.h location.hh position.hh stack.hh
CLEAN_EXTRA					= location.hh position.hh stack.hh  ice.lex.cpp ice.lex.h ice.tab.cpp ice.tab.hpp

LDFLAGS						+= "-L/usr/local/opt/flex/lib"
CPPFLAGS					+= "-I/usr/local/opt/flex/include"
LEX							= /usr/local/opt/flex/bin/flex

TARGET                      = Compiler.slib
Ice_LDLIBS					= -lfl

%/ice.tab.o:				FILE_WARNING_FLAGS += -Wno-uninitialized-const-reference
%/ice.lex.o:				FILE_WARNING_FLAGS += -Wno-sign-compare
%/Declaration.o:			FILE_WARNING_FLAGS += -Wno-unknown-pragmas
%/Action.o:					FILE_WARNING_FLAGS += -Wno-unknown-pragmas
%/Semantic.o:				FILE_WARNING_FLAGS += -Wno-unknown-pragmas
%/AssembleyGenerator.o:		FILE_WARNING_FLAGS += -Wno-unknown-pragmas
FILE_WARNING_FLAGS			= -Wno-unused-private-field

include $(THORSANVIL_ROOT)/build/tools/Makefile
