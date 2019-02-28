CXX=g++
CXXFLAGS=-Wall -pedantic -std=c++14
LDLIBS=-lwthttp -lwt
TARGETS=preprocessor server
RUNFLAGS=--docroot . --http-address 0.0.0.0 --http-port 8080


all: apprun

app:
	$(CXX) $(CXXFLAGS) $(LDLIBS) src/main.cpp src/WebApp.cpp src/kmp.cpp src/boolExp.cpp src/stemmer.cpp src/index.cpp -o webapp

apprun: app
	./webapp $(RUNFLAGS)

preprocessor:
	$(CXX) $(CXXFLAGS) src/preprocessor.cpp src/stemmer.cpp -o preprocessor

preprocessorrun: preprocessor
	./preprocessor

index:
	$(CXX) $(CXXFLAGS) src/index.cpp -o index

indexrun: index
	./index
