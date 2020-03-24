COMPILER = g++
CFLAGS = -g -MMD -MP -std=c++11 -O3 -fopenmp -pedantic -DWINSTD
WFLAGS = -pedantic -Wignored-qualifiers -Wreturn-type -Wmaybe-uninitialized -Wbool-compare -Wshadow -Wunused-but-set-variable
LIBS	= -lboost_system-mgw62-mt-x64-1_70 -lws2_32

SRCDIR = .
SHARE_DIR = ../share
LEARN_DIR = ./learn

SHARE_SOURCES = $(wildcard ./$(SHARE_DIR)/*.cpp)
SOURCES = $(wildcard *.cpp) 
LEARN_SOURCES = $(wildcard ./$(LEARN_DIR)/*.cpp)

OBJDIR = ./obj
SHARE_OBJDIR = ./$(SHARE_DIR)/obj
LEARN_OBJDIR = ./$(LEARN_DIR)/obj

OBJECTS = $(addprefix $(OBJDIR)/, $(notdir $(SOURCES:.cpp=.o)))
SHARE_OBJECTS = $(addprefix $(SHARE_OBJDIR)/, $(notdir $(SHARE_SOURCES:.cpp=.o)))
LEARN_OBJECTS = $(addprefix $(LEARN_OBJDIR)/, $(notdir $(LEARN_SOURCES:.cpp=.o)))

DEPENDS   = $(OBJECTS:.o=.d)
SHARE_DEPENDS = $(SHARE_OBJECTS:.o=.d)
LEARN_DEPENDS = $(LEARN_OBJECTS:.o=.d)

HEADERS = $(SOURCES:.cpp=.hpp)
SHARE_HEADERS = $(SHARE_SOURCES:.cpp=.hpp)
LEARN_HEADERS = $(LEARN_SOURCES:.cpp=.hpp)

TARGET = ai.dll

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(SRCDIR)/%.hpp
	$(COMPILER) $< -c $(CFLAGS) $(WFLAGS) $(LIBS) -o $@
	
$(SHARE_OBJDIR)/%.o: $(SHARE_DIR)/%.cpp $(SHARE_DIR)/%.hpp
	$(COMPILER) $< -c $(CFLAGS) $(WFLAGS) $(LIBS) -o $@
	
$(LEARN_OBJDIR)/%.o: $(LEARN_DIR)/%.cpp $(LEARN_DIR)/%.hpp
	$(COMPILER) $< -c $(CFLAGS) $(WFLAGS) $(LIBS) -o $@
	
$(TARGET): $(OBJECTS) $(SHARE_OBJECTS) $(LEARN_OBJECTS)
	$(COMPILER) -shared $(OBJECTS) $(SHARE_OBJECTS) $(LEARN_OBJECTS) $(CFLAGS) $(WFLAGS) $(LIBS) -o $@
	cp $(TARGET) ../
	
test:
	echo $(SHARE_OBJECTS)
	
test1:
	echo $(SHARE_DEPENDS)

clean:
	-rm -f $(OBJECTS) $(SHARE_OBJECTS) $(DEPENDS) $(SHARE_DEPENDS) $(TARGET) ai.dll

-include $(DEPENDS) $(SHARE_DEPENDS)
