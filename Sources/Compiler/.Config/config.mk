# File ../include/platform.mk must be included before!!!
CONFIGFILE=./.Config/config.sav.mk
FARMENUFILE=./.Config/list_make_param.wnd

include $(CONFIGFILE)

define PRINT-CONFIG
	@$(call PRINTLINE, CONFIG:)
	@$(call PRINTLINE, Work profile......$(profile))
	@$(call PRINTLINE, Work compiler.....$(comp))
	@$(call PRINTLINE, Stable compiler...$(scomp))
	@$(call PRINTLINE, Mode..............$(mode))
endef

define GEN-CONFIG
	@$(call PRINTLINE, Writting configuration.)
	@$(call PRINTLINE, profile = $(profile))> $(CONFIGFILE)
	@$(call PRINTLINE, comp = $(comp))>> $(CONFIGFILE)
	@$(call PRINTLINE, scomp = $(scomp))>> $(CONFIGFILE)
	@$(call PRINTLINE, mode = $(mode))>> $(CONFIGFILE)
endef

define CHECKPARAM
	$(call CHECKPARAM-AUX,$(strip $(1)))
endef

define CHECKPARAM-AUX
	$(if $($(1)_tr),,$(error Invalid value of variable $$$(1)=$($(1))))
endef

.PHONY: set print_config printconfig

set:
	$(GEN-CONFIG)
	$(PRINT-CONFIG)

print_config printconfig:
	$(PRINT-CONFIG)

#Default values

$(CONFIGFILE): profile  = CPPSR
$(CONFIGFILE): comp     = CPPSR
$(CONFIGFILE): scomp    = CPPSR
$(CONFIGFILE): mode     = MAKE

$(CONFIGFILE)::
	$(GEN-CONFIG)

.PHONY: resetconfig reset_config delete_config gen_config
.PHONY: $(FARMENUFILE)

resetconfig reset_config:: delete_config gen_config

delete_config::
	-$(RM) $(call FORMAT-PATH, $(CONFIGFILE))

gen_config::
	$(GEN-CONFIG)

$(FARMENUFILE)::
	@$(call PRINTLINE,C:  Продолжить) > $@
	@$(call PRINTLINE,    set COMP-SHORT=$(comp)) >> $@
	@$(call PRINTLINE,    set PROFILE-SHORT=$(profile)) >> $@
	@$(call PRINTLINE,    set PROFILE-ACRO=.cppsr.exe) >> $@
	@$(call PRINTLINE,    set SCOMP-SHORT=$(scomp)) >> $@
	@$(call PRINTLINE,    set MODE=$(mode)) >> $@
	@$(call PRINTLINE,    .Config\change_window default) >> $@
	@$(call PRINTLINE,--:) >> $@
	@$(call PRINTLINE,:   Компилятор $(comp)) >> $@
	@$(call PRINTLINE,:   Профиль $(profile)) >> $@
	@$(call PRINTLINE,:   Стабильный компилятор $(scomp)) >> $@
	@$(call PRINTLINE,:   Режим $(mode)) >> $@

comp-R5 = $(call BATEXE, ../Bin/start-r5)
comp-R5T = $(call BATEXE, ../Bin/start-r5-t)
comp-SR = $(call BATEXE, ../Bin/start-sr)
comp-CPPSR = $(call BATEXE, ../Bin/start-cpp)
comp-C++/SR = $(call BATEXE, ../Bin/start-cpp)

profile-R5 = mp-work-R5
profile-SR = mp-work-SR
profile-CPPSR = mp-work-CPPSR
profile-C++/SR = mp-work-CPPSR

scomp-R5 = refgo -c150 -l30 ../Bin/MRefal.s.rsl
scomp-SR = ..\Bin\mrefal.s.exe
scomp-CPPSR = ..\Bin\mrefal.s.cpp.exe
scomp-C++/SR = ..\Bin\mrefal.s.cpp.exe

sprofile-R5 = mp-stable-R5
sprofile-R5T = mp-stable-R5
sprofile-SR = mp-stable-SR
sprofile-CPPSR = mp-stable-CPPSR
sprofile-C++/SR = mp-stable-CPPSR

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
