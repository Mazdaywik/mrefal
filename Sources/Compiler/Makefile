all: work

PATH:=$(PATH):../../Bin

include ../Include/platform.mk
include .Config/config.mk
include Tests/details.mk

.PHONY: work stable check check-s menu help %.test %.test-s tests tests-s

work:
	$(call CHECKPARAM, comp)
	$(call CHECKPARAM, profile)
	$(call CHECKPARAM, mode)
	$(comp_tr) $(mode_tr) $(xlinx_tr) \
	  +cfg:.Config/config-work.cfg +MP:. +cfg:$(profile_tr).prj

stable:
	$(call CHECKPARAM, scomp)
	$(call CHECKPARAM, sprofile)
	$(call CHECKPARAM, mode)
	$(scomp_tr) $(mode_tr) $(xlinx_tr) \
	  +cfg:.Config/config-stable.cfg +MP:. +cfg:$(sprofile_tr).prj

check:
	$(MAKE) work
	@$(EMPTYLINE)
	$(MAKE) work

check-s:
	$(MAKE) stable
	@$(EMPTYLINE)
	$(MAKE) work

%.test:
	$(call CHECKPARAM, comp)
	$(call CHECKPARAM, profile)
	$(call CHECKPARAM, mode)
	$(comp_tr) $(mode_tr) $(xlinx_tr) +cfg:.Config/config-work.cfg +MP:. +be:CPPSR $*.mref ( -target:$*.exe )
	$*.exe

%.test-s:
	$(call CHECKPARAM, scomp)
	$(call CHECKPARAM, sprofile)
	$(call CHECKPARAM, mode)
	$(scomp_tr) $(mode_tr) $(xlinx_tr) +cfg:.Config/config-stable.cfg +MP:. +be:CPPSR $*.mref ( -target:$*.exe )
	$*.exe

TEST_SOURCES=$(wildcard Tests/*.mref)
TESTS=$(TEST_SOURCES:%.mref=%.test)
TESTS_S=$(TEST_SOURCES:%.mref=%.test-s)

tests: $(TESTS)
tests-s: $(TESTS_S)

menu: $(FARMENUFILE)

help:
	@echo Make options
	@echo   work
	@echo   stable
	@echo   check
	@echo   check-s
	@echo   menu
	@echo   help
