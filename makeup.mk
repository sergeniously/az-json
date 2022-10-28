#####################
# makeup project file
# ! Place here the project-specific code or leave it as is

include makeup/.mk

PROJECT_NAME=az-json
PROJECT_VERSION=1.0.0
PROJECT_COPYRIGHT=Belenkov Sergey
PROJECT_URL=https://github.com/sergeniously/az-json
PROJECT_EMAIL=sergeniously@mail.ru

# Import modules by default
$(call import_modules,Cpp)

# Default compiler options
$(call set_cxx_standard,c++2a)
$(call add_compile_options,-Wall)
