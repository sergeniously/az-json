include ../makeup.mk
$(call import_modules,Boost)

$(call find_boost,PATH:$(HOME)/boost LIBS:unit_test_framework)

ifeq ($(BOOST_LIBRARIES),)
$(warning boost unit test framework is not found)
else

$(call include_directories,$(ROOT_SOURCE_DIR)/headers $(BOOST_INCLUDE_DIR))
$(call link_directories,$(ROOT_BINARY_DIR)/sources $(BOOST_LIBRARY_DIR))
$(call link_libraries,az-json $(BOOST_LIBRARIES))

SOURCES=\
	PathTests.cpp \
	ValueTests.cpp \
	ReaderTests.cpp \
	WriterTests.cpp

PROGRAM=unit

$(call add_program,$(PROGRAM),$(SOURCES))

$(call add_dependencies,$(PROGRAM),../sources/libaz-json.a)

$(call add_test_program,unit-tests,$(PROGRAM))

endif
