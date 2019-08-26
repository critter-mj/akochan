COMPILER = g++
CFLAGS = -g -MMD -MP -std=c++11 -O3 -mwindows -fopenmp -DWINSTD

OPTGTK = 
LIBS = -lboost_system-mgw62-mt-x64-1_70 -lws2_32 -L./ -lai

SRCDIR = .
SHARE_DIR = share
GUI_DIR = gui

SHARE_SOURCES = $(wildcard ./$(SHARE_DIR)/*.cpp)
GUI_SOURCES =  $(wildcard ./$(GUI_DIR)/*.cpp)
SOURCES = $(wildcard *.cpp) 

OBJDIR = ./obj
GUI_OBJDIR = ./$(GUI_DIR)/obj
SHARE_OBJDIR = ./$(SHARE_DIR)/obj

OBJECTS = $(addprefix $(OBJDIR)/, $(notdir $(SOURCES:.cpp=.o)))
SHARE_OBJECTS = $(addprefix $(SHARE_OBJDIR)/, $(notdir $(SHARE_SOURCES:.cpp=.o)))
GUI_OBJECTS = $(addprefix $(GUI_OBJDIR)/, $(notdir $(GUI_SOURCES:.cpp=.o)))

DEPENDS   = $(OBJECTS:.o=.d)
SHARE_DEPENDS = $(SHARE_OBJECTS:.o=.d)
GUI_DEPENDS = $(GUI_OBJECTS:.o=.d)

HEADERS = $(SOURCES:.cpp=.hpp)
SHARE_HEADERS = $(SHARE_SOURCES:.cpp=.hpp)
GUI_HEADERS = $(GUI_SOURCES:.cpp=.hpp)

TARGET = gui.exe

$(TARGET): $(OBJECTS) $(SHARE_OBJECTS) $(GUI_OBJECTS)
	$(COMPILER) $(OBJECTS) $(SHARE_OBJECTS) $(GUI_OBJECTS) $(CFLAGS) $(OPTGTK) $(LIBS) -o $@
	
system.exe:  $(OBJECTS) $(SHARE_OBJECTS)
	$(COMPILER) $(OBJECTS) $(SHARE_OBJECTS) $(CFLAGS) $(LIBS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(SRCDIR)/%.hpp
	$(COMPILER) $< -c $(CFLAGS) $(OPTGTK) $(LIBS) -o $@
	
$(SHARE_OBJDIR)/%.o: $(SHARE_DIR)/%.cpp $(SHARE_DIR)/%.hpp
	$(COMPILER) $< -c $(CFLAGS) $(LIBS) -o $@
	
$(GUI_OBJDIR)/%.o: $(GUI_DIR)/%.cpp $(GUI_DIR)/%.hpp
	$(COMPILER) $< -c $(CFLAGS) $(OPTGTK) $(LIBS) -o $@
	
test:
	echo $(SHARE_OBJECTS)
	
test1:
	echo $(SHARE_DEPENDS)

clean:
	-rm -f $(OBJECTS) $(SHARE_OBJECTS) $(GUI_OBJECTS) $(DEPENDS) $(SHARE_DEPENDS) $(GUI_DEPENSDS) $(TARGET)

-include $(DEPENDS) $(SHARE_DEPENDS) &(GUI_DEPENDS)
