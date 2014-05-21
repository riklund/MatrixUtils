TARGETS:= Purify ConstructMatrix Jacobi
TARGETOBJ:=$(TARGETS:%=bin/%.o)

CC:=g++
INCLUDE=-Iinclude -IRLlib/include
LIBS= -LRLlib -lRLlib
LIBS+= -L/net/opt/intel/composer_xe_2013.0.079/mkl/lib/intel64 -lmkl_intel_lp64 -lmkl_core -lmkl_intel_thread -lm -liomp5
CCFLAGS=-Wall -DUSE_MKL_LAPACKE -DMKL_Complex16="std::complex<double>" -DMKL_INT="long" -DMKL_UINT="size_t" -pthread -g -O3 -DNO_STRICT_ASYMMETRY_CHECK

RLLIB:= RLlib/libRLlib.a


SRC:= $(wildcard src/*.cc)
OBJ:= $(SRC:src/%.cc=bin/%.o)

TEST:= test/RunTests

all: bin $(RLLIB) $(TARGETS) $(TEST)

bin: 
	mkdir -p bin

$(TARGETS): %: $(filter-out $(TARGETOBJ), $(OBJ)) bin/%.o
	@echo Compiling $@...
	@$(CC) $(CCFLAGS) $(INCLUDE) $^ $(LIBS) -o $@

$(OBJ): bin/%.o: src/%.cc include/%.hh
	@echo Compiling $@...
	@$(CC) $(CCFLAGS) $(INCLUDE) -c $< -o $@


clean: 
	@echo Cleaning up...
	@rm -rf bin/*
	@rm -rf $(TARGETS)
	@$(MAKE) -C RLlib clean
	@$(MAKE) -C test clean

$(RLLIB):
	$(MAKE) -C RLlib

$(TEST): $(OBJ)
	@$(MAKE) -C test makerun
