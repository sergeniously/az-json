include ../makeup.mk

SOURCES=\
	main.cpp

UTILITY=az-json

$(call include_directories,$(ROOT_SOURCE_DIR)/headers)
$(call link_directories,$(ROOT_BINARY_DIR)/sources)
$(call link_libraries,az-json)

$(call add_program,$(UTILITY),$(SOURCES))

$(call install_targets,$(UTILITY),$(ROOT_INSTALL_DIR)/usr/local/bin)
