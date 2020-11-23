LIBRARY=az-json
OUTPUT_DIR=binaries

SOURCES=\
	Error.cpp \
	Value.cpp \
	Path.cpp \
	Reader.cpp \
	Writer.cpp

OBJECTS=$(patsubst %.cpp, $(OUTPUT_DIR)/%.o, $(SOURCES))

BUILD_OPTIONS=\
	-std=c++2a -fPIC -g -O2 -Wall

STATIC_LIBRARY=$(OUTPUT_DIR)/lib$(LIBRARY).a
SHARED_LIBRARY=$(OUTPUT_DIR)/lib$(LIBRARY).so
TEST_UTILITY=$(OUTPUT_DIR)/test-$(LIBRARY)

all: $(OUTPUT_DIR) $(STATIC_LIBRARY) $(SHARED_LIBRARY)
	make -C tests

library: $(STATIC_LIBRARY) $(SHARED_LIBRARY)

$(OUTPUT_DIR):
	mkdir -p $@

$(STATIC_LIBRARY): $(OBJECTS)
	ar rcs $@ $(OBJECTS)

$(SHARED_LIBRARY): $(OBJECTS)
	g++ -shared -o $@ $(OBJECTS)

$(OBJECTS): $(OUTPUT_DIR)/%.o: %.h
$(OBJECTS): $(OUTPUT_DIR)/%.o: %.cpp
	g++ -c $< -o $@ $(BUILD_OPTIONS)

test:
	$(TEST_UTILITY) --no_color_output --detect_memory_leaks

leak:
	valgrind --leak-check=summary $(TEST_UTILITY)

clean:
	make -C tests clean
	rm -v -f $(OBJECTS) $(STATIC_LIBRARY) $(SHARED_LIBRARY)

.DEFAULT_GOAL := all
