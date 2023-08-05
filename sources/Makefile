include ../makeup.mk

$(call add_compile_options,-fPIC)
$(call include_directories,../headers)

$(call add_library,az-json,STATIC SHARED,\
	Error.cpp \
	Path.cpp \
	Reader.cpp \
	Value.cpp \
	Writer.cpp \
)

$(call install_targets,az-json,$(ROOT_INSTALL_DIR)/usr/local/lib)
