
CC := g++ # This is the main compiler

SRCDIR = src
BUILDDIR = build
TARGET = bin/emulator

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
SRC := $(SOURCES:$(SRCDIR)/%=%)
SRC := $(basename $(SRC))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))

CFLAGS :=
LIB :=
INC := -Iinclude

all: $(TARGET)

$(TARGET): $(SRC) #$(OBJECTS)
	@echo " Linking..."
	@echo " $(CC) $(OBJECTS) -o $(TARGET) $(LIB)"; $(CC) $(OBJECTS) -o $(TARGET) $(LIB)

$(SRC):
	@ echo " Building..."
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $(BUILDDIR)/$@.o $(SRCDIR)/$@.$(SRCEXT)"; $(CC) $(CFLAGS) $(INC) -c -o $(BUILDDIR)/$@.o $(SRCDIR)/$@.$(SRCEXT)

clean:
	@echo " Cleaning...";
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)

# Tests
tester:
	$(CC) $(CFLAGS) test/tester.cpp $(INC) $(LIB) -o bin/tester

# Spikes
#ticket:
#  $(CC) $(CFLAGS) spikes/ticket.cpp $(INC) $(LIB) -o bin/ticket

.PHONY: clean all
