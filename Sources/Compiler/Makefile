all: work

include ../Include/platform.mk
include .Config/config.mk

.PHONY: work stable check check-s

work:
	$(call CHECKPARAM, comp)
	$(call CHECKPARAM, profile)
	$(call CHECKPARAM, mode)
	$(comp_tr) $(mode_tr) +cfg:.Config/$(profile_tr).prj

stable:
	$(call CHECKPARAM, scomp)
	$(call CHECKPARAM, sprofile)
	$(call CHECKPARAM, mode)
	$(scomp_tr) $(mode_tr) +cfg:.Config/$(sprofile_tr).cfg +xlinx MRefal

check:
	$(MAKE) work
	@$(EMPTYLINE)
	$(MAKE) work

check-s:
	$(MAKE) stable
	@$(EMPTYLINE)
	$(MAKE) work

