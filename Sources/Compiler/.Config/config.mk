# File ../include/platform.mk must be included before!!!
CONFIGFILE=./.Config/config.sav.mk

include $(CONFIGFILE)

define PRINT-CONFIG
	@$(ECHO) CONFIG:
	@$(ECHO) Work profile......$(profile)
	@$(ECHO) Work compiler.....$(comp)
	@$(ECHO) Stable compiler...$(scomp)
	@$(ECHO) Mode..............$(mode)
endef

define GEN-CONFIG
	@$(EMPTYLINE)>> $(CONFIGFILE)
	@$(ECHO) profile = $(profile)>> $(CONFIGFILE)
	@$(ECHO) comp = $(comp)>> $(CONFIGFILE)
	@$(ECHO) scomp = $(scomp)>> $(CONFIGFILE)
	@$(ECHO) mode = $(mode)>> $(CONFIGFILE)
endef

define CHECKPARAM
	$(call CHECKPARAM-AUX,$(strip $(1)))
endef

define CHECKPARAM-AUX
	$(if $($(1)_tr),,$(error Invalid value of variable $$$(1)=$($(1))))
endef

.PHONY: set print_config printconfig

set: del_e
	$(GEN-CONFIG)
	$(PRINT-CONFIG)

print_config printconfig: del_e
	$(PRINT-CONFIG)

#Default values

$(CONFIGFILE): profile  = CPPSR
$(CONFIGFILE): comp     = CPPSR
$(CONFIGFILE): scomp    = CPPSR
$(CONFIGFILE): mode     = MAKE

$(CONFIGFILE): del_e
	$(GEN-CONFIG)

.PHONY: resetconfig reset_config delete_config gen_config

resetconfig reset_config:: delete_config gen_config

delete_config:: del_e
	-$(RM) $(call PATH, $(CONFIGFILE))

gen_config:: del_e
	$(GEN-CONFIG)

comp-R5 = $(call BATNAME, ../Bin/start-r5)
comp-R5T = $(call BATNAME, ../Bin/start-r5-t)
comp-SR = $(call BATNAME, ../Bin/start-sr)
comp-CPPSR = $(call BATNAME, ../Bin/start-cpp)

profile-R5 = mp-work-R5
profile-SR = mp-work-SR
profile-CPPSR = mp-work-CPPSR

scomp-R5 = refgo -c150 -l30 ../Bin/MRefal.s.rsl
scomp-SR = ..\Bin\mrefal.s.exe
scomp-CPPSR = ..\Bin\mrefal.s.cpp.exe

sprofile-R5 = mp-stable-R5
sprofile-R5T = mp-stable-R5
sprofile-SR = mp-stable-SR
sprofile-CPPSR = mp-stable-CPPSR

mode-MAKE = +make
mode-BUILD = +build
mode-FULL = +build
mode-LINKONLY = +linkonly

sprofile = $(comp)

comp_tr = $(comp-$(comp))
profile_tr = $(profile-$(profile))
scomp_tr = $(scomp-$(scomp))
sprofile_tr = $(sprofile-$(sprofile))
mode_tr = $(mode-$(mode))
