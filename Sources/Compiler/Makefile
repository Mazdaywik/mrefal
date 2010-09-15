include ../Include/platform.mk
include .Config/config.mk

.PHONY: work stable check check-s

all: work

work: del_e
	$(call CHECKPARAM, comp)
	$(call CHECKPARAM, profile)
	$(call CHECKPARAM, mode)
	$(comp_tr) $(mode_tr) +cfg:.Config/$(profile_tr).prj

stable: del_e
	$(call CHECKPARAM, scomp)
	$(call CHECKPARAM, sprofile)
	$(call CHECKPARAM, mode)
	$(scomp_tr) $(mode_tr) +cfg:.Config/$(sprofile_tr).cfg +xlinx MRefal

check: del_e
	$(MAKE) work
	@$(EMPTYLINE)
	$(MAKE) work

check-s: del_e
	$(MAKE) stable
	@$(EMPTYLINE)
	$(MAKE) work

