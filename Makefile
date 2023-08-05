include makeup.mk

$(call add_subdir_target,headers)
$(call add_subdir_target,library,DIR:sources)
$(call add_subdir_target,utility,DEPEND:library)

$(call add_test_directory,testing)
