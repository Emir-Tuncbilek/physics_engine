CONTEXT=sdl2
#CONTEXT=glfw3

CXXFLAGS += -DFENETRE_$(CONTEXT)
CXXFLAGS += -g -W -Wall -Wno-unused-parameter -Wvla -std=c++17 -pedantic # -Wno-deprecated-declarations
CXXFLAGS += $(shell pkg-config --cflags glew || echo -I/usr/local/include)
CXXFLAGS += $(shell pkg-config --cflags $(CONTEXT) || echo -I/usr/local/include/SDL2)

LDFLAGS += -g
LDFLAGS += $(shell pkg-config --libs glew || echo -I/usr/local/lib -lGLEW)
LDFLAGS += $(shell pkg-config --libs $(CONTEXT) || echo -I/usr/local/lib -lSDL2)
LDFLAGS += -lfreeimage
LDFLAGS += -ldl

ifeq "$(shell uname)" "Darwin"
  LDFLAGS += -framework OpenGL
  ifeq "$(CONTEXT)" "glfw3"
    LDFLAGS += -lobjc -framework Foundation -framework Cocoa
  endif
endif

SRC=main
BUILD=./build-$(shell uname)
EXE=$(BUILD)/physics.exe

SRC = $(wildcard *.cpp) $(wildcard */*.cpp)
OBJ = $(addprefix $(BUILD)/, $(notdir $(SRC:.cpp=.o)))

exe : $(EXE)
run : exe
	$(EXE)
$(EXE) : $(OBJ)
	$(CXX) $(CXXFLAGS) -o$@ $^ $(LDFLAGS)

$(BUILD)/%.o : %.cpp | $(BUILD)
	$(CC) $(CFLAGS) $(CXXFLAGS) -o $@ -c $<

$(BUILD)/%.o : */%.cpp | $(BUILD)
	$(CC) $(CFLAGS) $(CXXFLAGS) -o $@ -c $<

$(BUILD) :
	mkdir -p $@

cmake :
	mkdir -p $(BUILD)
	cd $(BUILD) && cmake ..

clean :
	rm -rf $(BUILD)/