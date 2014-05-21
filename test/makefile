#Modify this makefile to suit your needs.

#Create variables

CC:= g++

SRC:= $(wildcard src/*.cc)
OBJ:= $(SRC:src/%.cc=bin/%.o)


INCLUDE := -Iinclude -I../include 
#Add extra include directives here.
INCLUDE += -I../RLlib/include

#Add extra libraries here.
LIBS := -L../RLlib -lRLlib
LIBS+= -L/net/opt/intel/composer_xe_2013.0.079/mkl/lib/intel64 -lmkl_intel_lp64 -lmkl_core -lmkl_intel_thread -lm -liomp5

CCFLAGS=-O3 -Wall -DUSE_MKL_LAPACKE -DMKL_Complex16="std::complex<double>" -DMKL_INT="long" -DMKL_UINT="size_t" -pthread -g


MAINS:= RunTests
MAINSOBJ:= $(MAINS:%=bin/%.o)

#List all binaries in the main project which contains main() methods (these cannot be tested).
NON_TESTABLE_BIN:= ../bin/Purify.o ../bin/Jacobi.o ../bin/ConstructMatrix.o

TESTABLE_BIN:= $(filter-out $(NON_TESTABLE_BIN), $(wildcard ../bin/*.o))

#Targets.

.PHONY: clean update_headers all makerun

.NOTPARALLEL: update_headers include/ExistingTests.cff include/AllHeaders.cff

all: bin update_headers $(MAINS) temp

bin:
	@echo Making directory for binaries...
	@mkdir -p bin

temp:
	@echo Making temporary file directory...
	@mkdir -p temp

makerun: all
	@echo Running tests...
	@./RunTests

update_headers: 
	@./testlist_create.sh
	@./allheaders_create.sh

test: 
	@echo Compiling main test object...
	@$(MAKE) -C test makerun

include/ExistingTests.cff: testlist_create.sh
	@echo Creating list of existing tests...
	@./testlist_create.sh
	@touch include/ExistingTests.cff

include/AllHeaders.cff: allheaders_create.sh
	@echo Creating list of all other headers...
	@./allheaders_create.sh
	@touch include/AllHeaders.cff

$(MAINS): %: $(filter-out $(MAINSOBJ), $(OBJ)) include/ExistingTests.cff include/AllHeaders.cff bin/%.o $(TESTABLE_BIN)
	@echo Compiling $@
	@$(CC) $(CCFLAGS) $(INCLUDE) $(filter-out include/ExistingTests.cff include/AllHeaders.cff, $^) $(LIBS) -o $@  


bin/%.o: src/%.cc include/%.hh include/ExistingTests.cff include/AllHeaders.cff
	@echo Compiling $@
	@$(CC) $(CCFLAGS) $(INCLUDE) -c $< -o $@


clean:
	@echo Cleaning up test directories...
	@rm -rf $(MAINS)
	@rm -rf bin/*
	@rm -rf include/ExistingTests*.cff
	@rm -rf include/AllHeaders*.cff
	@rm -rf temp/*
	@find . -type f -name "*.o" -exec rm -f {} \;
	@find . -type f -name "*.a" -exec rm -f {} \;
	@find . -type f -name "*.d" -exec rm -f {} \;
	@find . -type f -name "*.of" -exec rm -f {} \;
	@find . -type f -name "*~" -exec rm -f {} \;
	@find . -type f -name "\#*\#" -exec rm -f {} \;
