CXX=g++
LD=g++
CXXFLAGS=-Wall -pedantic -std=c++14 -Wno-long-long -O0 -ggdb
LDLIBS=-lncurses

OBJECTS=answer.o handler.o main.o question.o quiz.o
OBJECTS+=ui.o uiCreateQuiz.o uiImportExport.o uiPrinter.o uiSolveQuiz.o
FILENAME=skluzada
VPATH = src

all: compile doc

compile: $(FILENAME)

$(FILENAME): $(OBJECTS)
	$(LD) -o $@ $^ $(LDLIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c -o $@ $<

run: $(FILENAME)
	./$(FILENAME)

clean:
	rm -rf $(OBJECTS) $(FILENAME) doc

doc:
	doxygen Doxyfile
	mv ./html ./doc

answer.o: answer.cpp answer.h
handler.o: handler.cpp handler.h quiz.h question.h answer.h
main.o: main.cpp ui.h handler.h quiz.h question.h answer.h uiPrinter.h uiSolveQuiz.h uiCreateQuiz.h uiImportExport.h
question.o: question.cpp question.h answer.h
quiz.o: quiz.cpp quiz.h question.h answer.h
ui.o: ui.cpp ui.h handler.h quiz.h question.h answer.h uiPrinter.h uiSolveQuiz.h uiCreateQuiz.h uiImportExport.h
uiCreateQuiz.o: uiCreateQuiz.cpp uiCreateQuiz.h handler.h quiz.h question.h answer.h uiPrinter.h
uiImportExport.o: uiImportExport.cpp uiImportExport.h handler.h quiz.h question.h answer.h uiPrinter.h
uiPrinter.o: uiPrinter.cpp uiPrinter.h
uiSolveQuiz.o: uiSolveQuiz.cpp uiSolveQuiz.h handler.h quiz.h question.h answer.h uiPrinter.h
