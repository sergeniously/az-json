DESTINATION=../binaries
APP=$(DESTINATION)/test-az-json
LIB=$(DESTINATION)/libaz-json.a

SOURCES=\
	ValueTests.cpp \
	ReaderTests.cpp \
	WriterTests.cpp \
	PathTests.cpp

OBJECTS=\
	$(patsubst %.cpp, $(DESTINATION)/%.o, $(SOURCES))

BUILD_OPTIONS=\
	-std=c++2a -fPIC -O2 -g -Wall

all: $(APP)
	valgrind --leak-check=summary --error-exitcode=1 $(APP)

$(APP): $(OBJECTS) $(LIB)
	g++ -o $@ $(OBJECTS) $(LIB) -lboost_unit_test_framework

$(OBJECTS): $(DESTINATION)/%.o: %.cpp
	g++ -c $< -o $@ $(BUILD_OPTIONS)

$(LIB):
	make -C ..

clean:
	rm -v -f $(OBJECTS) $(APP)

.DEFAULT_GOAL := all
