UNAME := $(shell uname 2>_e)

ifndef UNAME
  UNAME := Windows
endif

ifneq ($(findstring Windows, $(UNAME)),)
  OS=Windows
  CP=cmd.exe /c copy
  MV=cmd.exe /c move
  RM=cmd.exe /c del
  ECHO=cmd.exe /c echo
  EMPTYLINE=cmd.exe /c echo.
  EXESUF=.exe
  BATSUF=.bat
  PATH=$(strip $(subst /,\,$(1)))
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
  PATH=$(strip $(1))
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
  PATH=$(strip $(1))
endif

EXENAME=$(call PATH, $(1)$(EXESUF))
BATNAME=$(call PATH, $(1)$(BATSUF))

.PHONY: del_e

all: del_e

del_e::

del_e:: _e
	@$(RM) _e
