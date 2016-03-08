all: work

PATH:=$(PATH):../../Bin

include ../Include/platform.mk
include .Config/config.mk

.PHONY: work stable check check-s menu help

work:
	$(call CHECKPARAM, comp)
	$(call CHECKPARAM, profile)
	$(call CHECKPARAM, mode)
	$(comp_tr) $(mode_tr) $(xlinx_tr) +cfg:.Config/$(profile_tr).prj

stable:
	$(call CHECKPARAM, scomp)
	$(call CHECKPARAM, sprofile)
	$(call CHECKPARAM, mode)
	$(scomp_tr) $(mode_tr) $(xlinx_tr) +cfg:.Config/$(sprofile_tr).prj

check:
	$(MAKE) work
	@$(EMPTYLINE)
	$(MAKE) work

check-s:
	$(MAKE) stable
	@$(EMPTYLINE)
	$(MAKE) work

menu: $(FARMENUFILE)

help:
	@echo Make options
	@echo   work
	@echo   stable
	@echo   check
	@echo   check-s
	@echo   menu
	@echo   help
