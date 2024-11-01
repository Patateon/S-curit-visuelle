rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

ECHO = echo
BOLD = [1m
UNDERLINE = [4m
INVERSE = [7m
RESET = [0m
GREEN = [32m
RED = [31m
BLUE = [34m
CYAN = [36m
ORANGE = [38;5;208m

SOURCESCPP := $(call rwildcard,src,*.cpp)
SOURCESOBJ := $(SOURCESCPP:src/%.cpp=obj/%.o)

APPSCPP := $(call rwildcard,apps,*.cpp)
# APPSOBJ := $(APPSCPP:apps/%.cpp=obj/%.o)
APPSEXE := $(APPSCPP:apps/%.cpp=%.out)


DEPDIR := .deps
DEPFLAGS_BASE = -MT $@ -MMD -MP -MF $(DEPDIR)
DEPFLAGS = $(DEPFLAGS_BASE)/$*.d
DEPFLAGSMAIN = $(DEPFLAGS_BASE)/main.d


CFLAGS = -g -O3 -Wall -Iinclude 

.PRECIOUS: obj/%.o
.PHONY: clean

build : $(APPSEXE)

obj/%.o: src/%.cpp
	@g++ -c $(DEPFLAGS_BASE) $(DEPFLAGS) $(CFLAGS) src/$*.cpp -o obj/$*.o 
	@echo "$(BOLD)$(CYAN)Built source :$(RESET) $*"

%.out: apps/%.cpp $(SOURCESOBJ)
	@g++ $(DEPFLAGS_BASE) $(DEPFLAGS) $(CFLAGS) $(SOURCESOBJ) apps/$*.cpp -o $*.out
	@echo "$(UNDERLINE)$(BOLD)$(GREEN)Built app :$(RESET) $*"

rebuild : clean build 

clean :
	@rm -f $(APPSEXE) obj/*.o .deps/*.d 

