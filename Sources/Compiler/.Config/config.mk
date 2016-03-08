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
	@$(call PRINTLINE, XLinx.............$(xlinx))
endef

define GEN-CONFIG
	@$(call PRINTLINE, Writting configuration.)
	@$(call PRINTLINE, profile = $(profile))> $(CONFIGFILE)
	@$(call PRINTLINE, comp = $(comp))>> $(CONFIGFILE)
	@$(call PRINTLINE, scomp = $(scomp))>> $(CONFIGFILE)
	@$(call PRINTLINE, mode = $(mode))>> $(CONFIGFILE)
	@$(call PRINTLINE, xlinx = $(xlinx))>> $(CONFIGFILE)
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
	@$(call PRINTLINE,    set XLINX=$(xlinx)) >> $@
	@$(call PRINTLINE,    .Config\change_window default) >> $@
	@$(call PRINTLINE,--:) >> $@
	@$(call PRINTLINE,:   Компилятор $(comp)) >> $@
	@$(call PRINTLINE,:   Профиль $(profile)) >> $@
	@$(call PRINTLINE,:   Стабильный компилятор $(scomp)) >> $@
	@$(call PRINTLINE,:   Режим $(mode)) >> $@
	@$(call PRINTLINE,:   XLinx $(xlinx)) >> $@

comp-R5 = $(call BATEXE, ../Bin/start-r5)
comp-R5T = $(call BATEXE, ../Bin/start-r5-t)
comp-SR = $(call BATEXE, ../Bin/start-sr)
comp-CPPSR = $(call BATEXE, ../Bin/start-cpp)
comp-C++/SR = $(call BATEXE, ../Bin/start-cpp)

profile-R5 = work-mrefal-R5
profile-SR = work-mrefal-SR
profile-CPPSR = work-mrefal-CPPSR
profile-C++/SR = work-mrefal-CPPSR

scomp-R5 = refgo -c300 -l200 ../Bin/stable/MRefal.r5.rsl ++extent:..\Bin\stable\FS-Extent.cpp.exe
scomp-SR = ..\Bin\stable\MRefal.sr.exe
scomp-CPPSR = ..\Bin\stable\MRefal.cppsr.exe
scomp-C++/SR = ..\Bin\stable\MRefal.cppsr.exe

sprofile-R5 = stable/mrefal-R5
sprofile-R5T = stable/mrefal-R5
sprofile-SR = stable/mrefal-SR
sprofile-CPPSR = stable/mrefal-CPPSR
sprofile-C++/SR = stable/mrefal-CPPSR

mode-MAKE = +make
mode-BUILD = +build
mode-FULL = +build
mode-LINKONLY = +linkonly

xlinx-ON = +xlinx
xlinx-OFF =

sprofile = $(comp)

comp_tr = $(comp-$(comp))
profile_tr = $(profile-$(profile))
scomp_tr = $(scomp-$(scomp))
sprofile_tr = $(sprofile-$(sprofile))
mode_tr = $(mode-$(mode))
xlinx_tr = $(xlinx-$(xlinx))
