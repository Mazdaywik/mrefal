UNAME := $(shell uname)

ifndef UNAME
  UNAME := Windows
endif

ifneq ($(findstring Windows, $(UNAME)),)
  OS=Windows
  CMD=$(ComSpec)
  CP=$(CMD) /c copy
  MV=$(CMD) /c move
  RM=$(CMD) /c del
  ECHO=$(CMD) /c echo
  PRINTLINE=$(CMD) /c "echo $(1)"
  EMPTYLINE=$(CMD) /c echo.
  EXESUF=.exe
  BATSUF=.bat
  FORMAT-PATH=$(strip $(subst /,\,$(1)))
  BATEXE=$(ComSpec) /c $(call BATNAME,$(1))
endif

ifneq ($(findstring Linux, $(UNAME)),)
  OS=Linux
  CP=cp
  MV=mv
  RM=rm
  ECHO=echo
  PRINTLINE=echo "$(1)"
  EMPTYLINE=echo
  EXESUF=-lin
  BATSUF=.sh
  BATEXE=$(call BATNAME,$(1))
  FORMAT-PATH=$(strip $(1))
endif

ifneq ($(findstring QNX, $(UNAME)),)
  OS=QNX
  CP=cp
  MV=mv
  RM=rm
  ECHO=echo
  PRINTLINE=echo "$(1)"
  EMPTYLINE=echo
  EXESUF=-qnx
  BATSUF=.sh
  BATEXE=$(call BATNAME,$(1))
  FORMAT-PATH=$(strip $(1))
endif

EXENAME=$(call FORMAT-PATH, $(1)$(EXESUF))
BATNAME=$(call FORMAT-PATH, $(1)$(BATSUF))
