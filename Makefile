LIBRARY=az-json
DESTINATION=binaries

SOURCES=\
	Error.cpp \
	Value.cpp \
	Path.cpp \
	Reader.cpp \
	Writer.cpp

OBJECTS=$(patsubst %.cpp, $(DESTINATION)/%.o, $(SOURCES))

BUILD_OPTIONS=\
	-std=c++2a -fPIC -g -O2 -Wall

STATIC_LIBRARY=$(DESTINATION)/lib$(LIBRARY).a
SHARED_LIBRARY=$(DESTINATION)/lib$(LIBRARY).so

all: $(DESTINATION) $(STATIC_LIBRARY) $(SHARED_LIBRARY)

$(DESTINATION):
	mkdir -p $@

$(STATIC_LIBRARY): $(OBJECTS)
	ar rcs $@ $(OBJECTS)

$(SHARED_LIBRARY): $(OBJECTS)
	g++ -shared -o $@ $(OBJECTS)

$(OBJECTS): $(DESTINATION)/%.o: %.cpp
	g++ -c $< -o $@ $(BUILD_OPTIONS)

check: $(STATIC_LIBRARY)
	make -C tests

clean:
	make -C tests clean
	rm -v -f $(OBJECTS) $(STATIC_LIBRARY) $(SHARED_LIBRARY)

.DEFAULT_GOAL := all
