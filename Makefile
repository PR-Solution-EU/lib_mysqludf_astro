########################################################################
############################# Makefile  ################################
########################################################################
# Compiler settings
CC = g++
CXXFLAGS = -Wall -shared -I/usr/include/mysql -fPIC
LDFLAGS = -ljsonparser

# Makefile settings
LIBNAME = lib_mysqludf_astro.so
EXT = .cpp
SRCDIR = src
OBJDIR = obj

LIBDIR = /usr/lib
MYSQLPLUGINDIR = $(LIBDIR)/mysql/plugin

############## Do not change anything from here downwards! #############
SRC = $(wildcard $(SRCDIR)/*$(EXT))
OBJ = $(SRC:$(SRCDIR)/%$(EXT)=$(OBJDIR)/%.o)
DEP = $(OBJ:$(OBJDIR)/%.o=%.d)

RM = rm
CP = cp
LN = ln
DELOBJ = $(OBJ)

########################################################################
####################### Targets beginning here #########################
########################################################################

all: $(LIBNAME)

# Builds the app
$(LIBNAME): $(OBJ)
	$(CC) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Creates the dependecy rules
%.d: $(SRCDIR)/%$(EXT)
	@$(CPP) $(CFLAGS) $< -MM -MT $(@:%.d=$(OBJDIR)/%.o) >$@

# Includes all .h files
-include $(DEP)

# Building rule for .o files and its .c/.cpp in combination with all .h
$(OBJDIR)/%.o: $(SRCDIR)/%$(EXT)
	$(CC) $(CXXFLAGS) -o $@ -c $<

# Cleans complete project
.PHONY: clean
clean:
	$(RM) $(DELOBJ) $(DEP) $(LIBNAME)

# Cleans only all files with the extension .d
.PHONY: cleandep
cleandep:
	$(RM) $(DEP)

.PHONY: install
install:
	$(CP) $(LIBNAME) $(LIBDIR)/$(LIBNAME)
	$(LN) -fs $(LIBDIR)/$(LIBNAME) $(MYSQLPLUGINDIR)/$(LIBNAME)

.PHONY: uninstall
uninstall:
	$(RM) $(MYSQLPLUGINDIR)/$(LIBNAME)
	$(RM) $(LIBDIR)/$(LIBNAME)
