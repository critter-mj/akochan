COMPILER = g++
CFLAGS = -g -MMD -MP -std=c++11 -O3 -fopenmp -DWINSTD
WFLAGS = -pedantic -Wignored-qualifiers -Wreturn-type -Wmaybe-uninitialized -Wbool-compare -Wshadow -Wunused-but-set-variable -Wunused-variable

OPTGTK = 
LIBS = -lboost_system-mgw62-mt-x64-1_70 -lws2_32 -L./ -lai

SRCDIR = .
SHARE_DIR = share

SHARE_SOURCES = $(wildcard ./$(SHARE_DIR)/*.cpp)
SOURCES = $(wildcard *.cpp) 

OBJDIR = ./obj
SHARE_OBJDIR = ./$(SHARE_DIR)/obj

OBJECTS = $(addprefix $(OBJDIR)/, $(notdir $(SOURCES:.cpp=.o)))
SHARE_OBJECTS = $(addprefix $(SHARE_OBJDIR)/, $(notdir $(SHARE_SOURCES:.cpp=.o)))

DEPENDS   = $(OBJECTS:.o=.d)
SHARE_DEPENDS = $(SHARE_OBJECTS:.o=.d)

HEADERS = $(SOURCES:.cpp=.hpp)
SHARE_HEADERS = $(SHARE_SOURCES:.cpp=.hpp)

TARGET = system.exe
	
$(TARGET):  $(OBJECTS) $(SHARE_OBJECTS)
	$(COMPILER) $(OBJECTS) $(SHARE_OBJECTS) $(CFLAGS) $(WFLAGS) $(LIBS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(SRCDIR)/%.hpp
	$(COMPILER) $< -c $(CFLAGS) $(WFLAGS) $(OPTGTK) $(LIBS) -o $@
	
$(SHARE_OBJDIR)/%.o: $(SHARE_DIR)/%.cpp $(SHARE_DIR)/%.hpp
	$(COMPILER) $< -c $(CFLAGS) $(WFLAGS) $(LIBS) -o $@
	
test:
	echo $(SHARE_OBJECTS)
	
test1:
	echo $(SHARE_DEPENDS)

clean:
	-rm -f $(OBJECTS) $(SHARE_OBJECTS) $(DEPENDS) $(SHARE_DEPENDS) $(TARGET)

-include $(DEPENDS) $(SHARE_DEPENDS)
