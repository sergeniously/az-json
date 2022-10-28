include ../makeup.mk
$(call import_modules,Boost)

$(call find_boost,PATH:$(HOME)/boost LIBS:unit_test_framework)

ifeq ($(BOOST_FOUND),no)
$(info boost unit test framework is not found)
else

$(call include_directories,$(ROOT_SOURCE_DIR)/headers $(BOOST_INCLUDE_DIR))
$(call link_directories,$(ROOT_BINARY_DIR)/sources $(BOOST_LIBRARY_DIR))
$(call link_libraries,az-json $(BOOST_LIBRARIES))

SOURCES=\
	PathTests.cpp \
	ReaderTests.cpp \
	ValueTests.cpp \
	WriterTests.cpp

$(call add_program,az-json-test,$(SOURCES))

$(call add_dependencies,az-json-test,../sources/libaz-json.a)

$(call add_test_program,unit-tests,az-json-test)

endif
