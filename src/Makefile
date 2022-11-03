CXX = g++
CXXFLAGS = -std=c++17 -O3 -march=native
# CXXFLAGS = -std=c++17 -pg -g  -Wall -Wextra -pedantic -DDEBUG
# CXXFLAGS = -std=c++17 -g  -Wall -Wextra -pedantic -DDEBUG

LIBS = cell.o \
	   fm_parse.o \
	   fm_partition.o \
	   fm_output.o \
	   fm_cut.o \
	   fm_metadata.o \

RES = main.cpp
EXE = ../bin/hw2
TESTCASE_DIR = ../testcases
OUTPUT_DIR = ../output
VERIFY_DIR = ../verifier

.PHONY: test clean

CASE = 1

all: $(LIBS)
	$(CXX) $(CXXFLAGS) $(RES) $(LIBS) -o $(EXE)
	-rm $(LIBS)

test:
	-rm $(OUTPUT_DIR)/*.out
	time ./$(EXE) $(TESTCASE_DIR)/p2-$(CASE).cells $(TESTCASE_DIR)/p2-$(CASE).nets $(OUTPUT_DIR)/p2-$(CASE).out
	./$(VERIFY_DIR)/verify $(TESTCASE_DIR)/p2-$(CASE).cells $(TESTCASE_DIR)/p2-$(CASE).nets $(OUTPUT_DIR)/p2-$(CASE).out

testall:
	-rm $(OUTPUT_DIR)/*.out
	time ./$(EXE) $(TESTCASE_DIR)/p2-0.cells $(TESTCASE_DIR)/p2-0.nets $(OUTPUT_DIR)/p2-0.out
	./$(VERIFY_DIR)/verify $(TESTCASE_DIR)/p2-0.cells $(TESTCASE_DIR)/p2-0.nets $(OUTPUT_DIR)/p2-0.out
	time ./$(EXE) $(TESTCASE_DIR)/p2-1.cells $(TESTCASE_DIR)/p2-1.nets $(OUTPUT_DIR)/p2-1.out
	./$(VERIFY_DIR)/verify $(TESTCASE_DIR)/p2-1.cells $(TESTCASE_DIR)/p2-1.nets $(OUTPUT_DIR)/p2-1.out
	time ./$(EXE) $(TESTCASE_DIR)/p2-2.cells $(TESTCASE_DIR)/p2-2.nets $(OUTPUT_DIR)/p2-2.out
	./$(VERIFY_DIR)/verify $(TESTCASE_DIR)/p2-2.cells $(TESTCASE_DIR)/p2-2.nets $(OUTPUT_DIR)/p2-2.out
	time ./$(EXE) $(TESTCASE_DIR)/p2-3.cells $(TESTCASE_DIR)/p2-3.nets $(OUTPUT_DIR)/p2-3.out
	./$(VERIFY_DIR)/verify $(TESTCASE_DIR)/p2-3.cells $(TESTCASE_DIR)/p2-3.nets $(OUTPUT_DIR)/p2-3.out
	time ./$(EXE) $(TESTCASE_DIR)/p2-4.cells $(TESTCASE_DIR)/p2-4.nets $(OUTPUT_DIR)/p2-4.out
	./$(VERIFY_DIR)/verify $(TESTCASE_DIR)/p2-4.cells $(TESTCASE_DIR)/p2-4.nets $(OUTPUT_DIR)/p2-4.out
	time ./$(EXE) $(TESTCASE_DIR)/p2-5.cells $(TESTCASE_DIR)/p2-5.nets $(OUTPUT_DIR)/p2-5.out
	./$(VERIFY_DIR)/verify $(TESTCASE_DIR)/p2-5.cells $(TESTCASE_DIR)/p2-5.nets $(OUTPUT_DIR)/p2-5.out

clean:
	-rm $(EXE)
	-rm $(LIBS)
	-rm $(OUTPUT_DIR)/*.out

benchmark: packing_gz
	mv CS6135_HW2_111062506.tar.gz ../HW2_grading/student/111062506/
	cd ../HW2_grading && bash HW2_grading.sh

upload: packing_gz
	ssh g111062506@ic 'rm -rf ~/hw2/HW2_grading'
	scp -r ../HW2_grading g111062506@ic:~/hw2
	scp CS6135_HW2_111062506.tar.gz g111062506@ic:~/hw2/HW2_grading/student/111062506/
	cd .. && scp -r src g111062506@ic:~/hw2/HW2_grading/

packing_gz: all
	-rm -rf HW2 CS6135_HW2_111062506.tar.gz
	mkdir -p HW2/src
	cp *.cpp HW2/src/
	cp *.h HW2/src/
	cp -r ../output/ HW2/
	cp -r ../bin/ HW2/
	tar -zcvf CS6135_HW2_111062506.tar.gz HW2/
	-rm -rf HW2

# send_cases:
# 	scp -r testcases g111062506@ic:~/hw2

# send:
# 	scp -r src u107062115@ic:~/hw2

# packing:
# 	-rm -rf CS3130_PA2 PA2_107062115.tar.gz
# 	mkdir CS3130_PA2
# 	cp -r src/ CS3130_PA2/
# 	cp Makefile CS3130_PA2
# 	tar -zcvf PA2_107062115.tar.gz CS3130_PA2/
# 	-rm -rf CS3130_PA2
