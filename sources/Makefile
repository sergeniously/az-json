include ../makeup.mk

SOURCES=\
	Error.cpp \
	Value.cpp \
	Reader.cpp \
	Writer.cpp \
	Path.cpp

$(call add_compile_options,-fPIC)
$(call include_directories,../headers)

$(call add_library,az-json,STATIC SHARED,$(SOURCES))

$(call install_targets,az-json,$(ROOT_INSTALL_DIR)/usr/local/lib)
