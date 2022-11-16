CXX = g++
CXXFLAGS = -std=c++11 -O3 -march=native
# CXXFLAGS = -std=c++17 -pg -g  -Wall -Wextra -pedantic -DDEBUG
# CXXFLAGS = -std=c++17 -g  -Wall -Wextra -pedantic -DDEBUG

LIBS = fm_parse.o \
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

testall: clean all
	-rm $(OUTPUT_DIR)/*.out
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

benchmark: packing_gz
	-rm -rf ../HW2_grading
	tar -zxvf ../HW2_grading.tar.gz -C ../
	mkdir -p ../HW2_grading/student/111062506
	cp CS6135_HW2_111062506.tar.gz ../HW2_grading/student/111062506/
	cd ../HW2_grading && bash HW2_grading.sh

upload: packing_gz
	ssh g111062506@ic 'rm -rf ~/hw2/HW2_grading'
	scp ../HW2_grading.tar.gz g111062506@ic:~/hw2/
	ssh g111062506@ic 'cd hw2/ && tar -zxvf HW2_grading.tar.gz && mkdir HW2_grading/student/111062506'
	scp CS6135_HW2_111062506.tar.gz g111062506@ic:~/hw2/HW2_grading/student/111062506/

submit: testall packing_gz benchmark
	echo "finish"

packing_gz: testall
	-rm -rf HW2 CS6135_HW2_111062506.tar.gz
	mkdir -p HW2/src
	cp *.cpp HW2/src/
	cp *.h HW2/src/
	cp Makefile HW2/src/
	cp README HW2/src/
	cp -r ../output/ HW2/
	mkdir -p HW2/bin
	cp ../bin/hw2 HW2/bin/hw2
	cp ../vlsi-hw2.pdf HW2/CS6135_HW2_111062506_report.pdf
	echo "below is unneccesary"
	cp -r ../testcases HW2/
	cp -r ../verifier HW2/
	tar -zcvf CS6135_HW2_111062506.tar.gz HW2/
	cp CS6135_HW2_111062506.tar.gz ../
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
