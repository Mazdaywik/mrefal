UNAME := $(shell uname 2>_e)

ifndef UNAME
  UNAME := Windows
endif

ifneq ($(findstring Windows, $(UNAME)),)
  OS=Windows
  CP=$(ComSpec) /c copy
  MV=$(ComSpec) /c move
  RM=$(ComSpec) /c del
  ECHO=$(ComSpec) /c echo
  EMPTYLINE=$(ComSpec) /c echo.
  EXESUF=.exe
  BATSUF=.bat
  FORMAT-PATH=$(strip $(subst /,\,$(1)))
endif

ifneq ($(findstring Linux, $(UNAME)),)
  OS=Linux
  CP=cp
  MV=mv
  RM=rm
  ECHO=echo
  EMPTYLINE=echo
  EXESUF=-lin
  BATSUF=.sh
  FORMAT-PATH=$(strip $(1))
endif

ifneq ($(findstring QNX, $(UNAME)),)
  OS=QNX
  CP=cp
  MV=mv
  RM=rm
  ECHO=echo
  EMPTYLINE=echo
  EXESUF=-qnx
  BATSUF=.sh
  FORMAT-PATH=$(strip $(1))
endif

EXENAME=$(call FORMAT-PATH, $(1)$(EXESUF))
BATNAME=$(call FORMAT-PATH, $(1)$(BATSUF))

.PHONY: del_e

all: del_e

del_e::

del_e:: _e
	@$(RM) _e
