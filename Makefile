CC = g++
CCFLAGS = -g -W -Wall -Wno-unused-parameter -Wno-write-strings
FLEX = flex
FLEX_OPTS = -Pvhdl
BISON = bison
BISON_OPTS = -t -pvhdl
BNFC = bnfc
OPTIMISATION_FLAGS = -O2

OBJS = Absyn.o Lexer.o Parser.o Printer.o RuleCheck.o Simulator.o

.PHONY: clean

clean:
	rm -f Absyn.C Absyn.H Test.C Parser.C Parser.H Lexer.C Skeleton.C Skeleton.H Printer.C Printer.H vhdl.l vhdl.y *.bak *.o TestSyntax RudeSim

Absyn.H Absyn.C javalette.l javalette.y Parser.H Skeleton.H Skeleton.C Printer.H Printer.C Test.C: src/vhdl.cf
	${BNFC} -cpp_stl src/vhdl.cf


Absyn.o: Absyn.C Absyn.H
	${CC} ${CCFLAGS} -c Absyn.C

Lexer.C: vhdl.l
	${FLEX} -o Lexer.C vhdl.l

Parser.C: vhdl.y
	${BISON} vhdl.y -o Parser.C

Lexer.o: Lexer.C Parser.H
	${CC} ${CCFLAGS} -c Lexer.C

Parser.o: Parser.C Absyn.H
	${CC} ${CCFLAGS} -c Parser.C

Printer.o: Printer.C Printer.H Absyn.H
	${CC} ${CCFLAGS} -c Printer.C

Skeleton.o: Skeleton.C Skeleton.H Absyn.H
	${CC} ${CCFLAGS} -c Skeleton.C

RuleCheck.o: src/RuleCheck.cpp src/RuleCheck.H Absyn.H Absyn.C
	${CC} ${CCFLAGS} -c src/RuleCheck.cpp

Simulator.o: src/Simulator.cpp src/Simulator.H Absyn.H Absyn.C
	${CC} ${CCFLAGS} -c src/Simulator.cpp

RudeSim.o: src/RudeSim.cpp Parser.H Printer.H Absyn.H Absyn.C src/RuleCheck.cpp src/RuleCheck.H src/Simulator.cpp src/Simulator.H
	${CC} ${CCFLAGS} -c src/RudeSim.cpp

RudeSim: ${OBJS} RudeSim.o
	${CC} ${CCFLAGS} ${OBJS} RudeSim.o -o RudeSim

Test.o: Test.C Parser.H Printer.H Absyn.H
	${CC} ${CCFLAGS} -c Test.C

TestSyntax: ${OBJS} Test.o
	${CC} ${CCFLAGS} ${OBJS} Test.o -o TestSyntax
	cat tests/testfile.vhdl | ./TestSyntax

#TestRudeSim: RudeSim
#	cat tests/testfile.vhdl | ./RudeSim -d

TestParsing: RudeSim
	./RudeSim -f "tests/testfile.vhdl,tests/testfile2.vhdl,tests/testfile3.vhdl" -dp
TestRulechecking: RudeSim
	./RudeSim -f "tests/testfile.vhdl,tests/testfile2.vhdl,tests/testfile3.vhdl" -dpc
