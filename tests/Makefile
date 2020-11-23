OUTPUT_DIR=../binaries
APP=$(OUTPUT_DIR)/test-az-json
LIB=$(OUTPUT_DIR)/libaz-json.a

SOURCES=\
	test_value.cpp \
	test_reader.cpp \
	test_writer.cpp \
	test_path.cpp \
	test.cpp

OBJECTS=\
	$(patsubst %.cpp, $(OUTPUT_DIR)/%.o, $(SOURCES))

BUILD_OPTIONS=\
	-std=c++2a -fPIC -O2 -g -Wall

all: $(APP)

$(APP): $(OBJECTS) $(LIB)
	g++ -o $@ $(OBJECTS) $(LIB) -lboost_unit_test_framework

$(OBJECTS): $(OUTPUT_DIR)/%.o: %.cpp
	g++ -c $< -o $@ $(BUILD_OPTIONS)

clean:
	rm -v -f $(OBJECTS) $(APP)

.DEFAULT_GOAL := all
