include makeup.mk

$(call add_subdir_target,library,DIR:sources)

$(call add_test_directory,tests)
