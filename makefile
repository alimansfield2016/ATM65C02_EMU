########################################################################
####################### Makefile Template ##############################
########################################################################

# Compiler settings - Can be customized.

CC = g++
CXXFLAGS = -Wall 
INCLUDEPATH = -I/home/ali/include/
LDFLAGS = -lX11 -lGL -lpthread -lpng -lstdc++fs
DBFLAGS = -ggdb

# Makefile settings - Can be customized.
APPNAME = myapp
EXT = .cpp
SRCDIR = src
OBJDIR = obj
DEPDIR = dep

PEDA_SESS:=${wildcard peda-session*}


################# pandoc markdown conversion stuff ################
SOURCE_DOCS := $(wildcard *.md)

EXPORTED_DOCS=\
 $(SOURCE_DOCS:.md=.html) \
 $(SOURCE_DOCS:.md=.pdf) \
 $(SOURCE_DOCS:.md=.docx) \
 $(SOURCE_DOCS:.md=.rtf) \
 $(SOURCE_DOCS:.md=.odt) \
 $(SOURCE_DOCS:.md=.epub)

PDF_ONLY=\
 $(SOURCE_DOCS:.md=.pdf) 

HTML_ONLY=\
 $(SOURCE_DOCS:.md=.html) \

RM=/bin/rm

PANDOC=/usr/bin/pandoc

PANDOC_OPTIONS= --standalone

PANDOC_HTML_OPTIONS=--to html5 --from markdown+yaml_metadata_block+raw_html --highlight-style breezedark -V geometry:margin=2cm 
PANDOC_PDF_OPTIONS=--from markdown+yaml_metadata_block+raw_html --highlight-style breezedark -V geometry:margin=2cm --pdf-engine=xelatex
PANDOC_DOCX_OPTIONS=
PANDOC_RTF_OPTIONS=
PANDOC_ODT_OPTIONS=
PANDOC_EPUB_OPTIONS=--to epub3


# Pattern-matching Rules
%.html : %.md
	$(PANDOC) $(PANDOC_OPTIONS) $(PANDOC_HTML_OPTIONS) -o $@ $<

%.pdf : %.md
	$(PANDOC) $(PANDOC_OPTIONS) $(PANDOC_PDF_OPTIONS) -o $@ $<
	
%.docx : %.md
	$(PANDOC) $(PANDOC_OPTIONS) $(PANDOC_DOCX_OPTIONS) -o $@ $<

%.rtf : %.md
	$(PANDOC) $(PANDOC_OPTIONS) $(PANDOC_RTF_OPTIONS) -o $@ $<

%.odt : %.md
	$(PANDOC) $(PANDOC_OPTIONS) $(PANDOC_ODT_OPTIONS) -o $@ $<

%.epub : %.md
	$(PANDOC) $(PANDOC_OPTIONS) $(PANDOC_EPUB_OPTIONS) -o $@ $<

############## Do not change anything from here downwards! #############
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
	$(RM) $(DELOBJ) $(DEP) $(APPNAME) $(EXPORTED_DOCS) ${PEDA_SESS}

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

.PHONY:  pdf html

#all : $(EXPORTED_DOCS)

pdf : $(PDF_ONLY)

html : ${HTML_ONLY}

#clean:
#	- $(RM) $(EXPORTED_DOCS)
