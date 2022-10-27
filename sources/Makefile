include ../makeup.mk

$(call include_directories,../headers)

$(call add_library,az-json,STATIC SHARED,\
	Error.cpp \
	Path.cpp \
	Reader.cpp \
	Value.cpp \
	Writer.cpp \
)

$(call install_targets,az-json,$(ROOT_INSTALL_DIR))
