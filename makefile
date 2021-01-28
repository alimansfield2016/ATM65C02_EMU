########################################################################
####################### Makefile Template ##############################
########################################################################

# Compiler settings - Can be customized.

CC = g++
CXXFLAGS = -Wall 
INCLUDEPATH = -I./src/include/
LDFLAGS = -lX11 -lGL -lpthread -lpng -lstdc++fs
DBFLAGS = -ggdb

# Makefile settings - Can be customized.
APPNAME = ATM65C02_EMU
EXT = .cpp
SRCDIR = src
OBJDIR = obj
DEPDIR = dep

RM=/bin/rm

SRC = $(wildcard $(SRCDIR)/*$(EXT))
OBJ = $(SRC:$(SRCDIR)/%$(EXT)=$(OBJDIR)/%.o)
DEP = $(OBJ:$(OBJDIR)/%.o=${DEPDIR}/%.d)
# UNIX-based OS variables & settings
RM = rm
DELOBJ = $(OBJ)
RMFLAGS = 2> /dev/null
# Windows OS variables & settings
DEL = del
EXE = .exe
WDELOBJ = $(SRC:$(SRCDIR)/%$(EXT)=$(OBJDIR)\\%.o)

########################################################################
####################### Targets beginning here #########################
########################################################################


all: $(APPNAME)
	mkdir ./dep -p
	mkdir ./obj -p

# Builds the app
$(APPNAME): $(OBJ)
	$(CC) $(CXXFLAGS) ${INCLUDEPATH} -o $@ $^ $(LDFLAGS) ${DBFLAGS}

# Creates the dependecy rules
${DEPDIR}/%.d: $(SRCDIR)/%$(EXT)
	@$(CPP) $(CFLAGS) ${INCLUDEPATH} $< -MM -MT $(@:${DEPDIR}%.d=$(OBJDIR)/%.o) >$@

# Includes all .h files
-include $(DEP)

# Building rule for .o files and its .c/.cpp in combination with all .h
$(OBJDIR)/%.o: $(SRCDIR)/%$(EXT)
	$(CC) $(CXXFLAGS) ${INCLUDEPATH} ${DBFLAGS} -o $@ -c $<

################### Cleaning rules for Unix-based OS ###################
# Cleans complete project
.PHONY: clean
clean:
	$(RM) $(DELOBJ) $(DEP) $(APPNAME) # $(EXPORTED_DOCS) ${PEDA_SESS}

# Cleans only all files with the extension .d
.PHONY: cleandep
cleandep:
	$(RM) $(DEP)

#################### Cleaning rules for Windows OS #####################
# Cleans complete project
.PHONY: cleanw
cleanw:
	$(DEL) $(WDELOBJ) $(DEP) $(APPNAME)$(EXE)

# Cleans only all files with the extension .d
.PHONY: cleandepw
cleandepw:
	$(DEL) $(DEP)
