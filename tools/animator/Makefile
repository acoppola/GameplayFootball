CPP  = ccache g++
RES  =
OBJ  = src/main.o src/animator.o src/gui/button.o src/gui/caption.o src/gui/filebrowser.o src/gui/filedialog.o src/gui/gui.o src/gui/guiinterface.o src/gui/textinput.o src/gui/timeline.o src/gui/view.o $(RES)
#LIBS =  -L"D:/mingw/lib" -nostdlibs -ltcmalloc_minimal -lm -lgcc  ../../blunted.a  -lSDL_gfx -lSDL_image -lSDL_ttf -lSGE -lSDL -lSDLmain  -lopengl32 -lglu32 -lOpenAL32  d:/libs/boost_1_49_0/stage/lib/libboost_thread-mgw46-mt-1_49.dll.a d:/libs/boost_1_49_0/stage/lib/libboost_signals-mgw46-mt-1_49.dll.a d:/libs/boost_1_49_0/stage/lib/libboost_filesystem-mgw46-mt-1_49.dll.a d:/libs/boost_1_49_0/stage/lib/libboost_system-mgw46-mt-1_49.dll.a  -lwinmm -lgdi32  -s
LIBS = -L"/lib" -L"/usr/lib" ../../blunted.a -lSDLmain -lSDL -lSDL_gfx -lSDL_image -lSDL_ttf -lSGE -lGL -lGLU -lopenal -lboost_thread -lboost_signals -lboost_filesystem -lboost_system
CXXINCS = -I"../../src" -I"/usr/include/SDL"  -I"src"  -I"/usr/include" -I"../../src" -L"/usr/lib"
BIN  = animator
CXXFLAGS = $(CXXINCS) -DBOOST_ENABLE_ASSERT_HANDLER -DNxDEBUG -DBxOOST_DISABLE_ASSERTS -DBOOST_SIGNALS_NO_DEPRECATION_WARNING -pthread -Wall -Wno-unused-variable -Wno-unused-local-typedefs -Wno-narrowing -fno-strict-aliasing -fprefetch-loop-arrays -fsignaling-nans -mmmx -msse2 -fexpensive-optimizations -O2
RM = rm -f

.PHONY: all all-before all-after clean clean-custom

all: all-before animator all-after


clean: clean-custom
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CPP) $(OBJ) -o $(BIN) $(LIBS)


src/main.o: src/main.cpp
	$(CPP) -c src/main.cpp -o src/main.o $(CXXFLAGS)

src/animator.o: src/animator.cpp
	$(CPP) -c src/animator.cpp -o src/animator.o $(CXXFLAGS)

src/gui/button.o: src/gui/button.cpp
	$(CPP) -c src/gui/button.cpp -o src/gui/button.o $(CXXFLAGS)

src/gui/caption.o: src/gui/caption.cpp
	$(CPP) -c src/gui/caption.cpp -o src/gui/caption.o $(CXXFLAGS)

src/gui/filebrowser.o: src/gui/filebrowser.cpp
	$(CPP) -c src/gui/filebrowser.cpp -o src/gui/filebrowser.o $(CXXFLAGS)

src/gui/filedialog.o: src/gui/filedialog.cpp
	$(CPP) -c src/gui/filedialog.cpp -o src/gui/filedialog.o $(CXXFLAGS)

src/gui/gui.o: src/gui/gui.cpp
	$(CPP) -c src/gui/gui.cpp -o src/gui/gui.o $(CXXFLAGS)

src/gui/guiinterface.o: src/gui/guiinterface.cpp
	$(CPP) -c src/gui/guiinterface.cpp -o src/gui/guiinterface.o $(CXXFLAGS)

src/gui/textinput.o: src/gui/textinput.cpp
	$(CPP) -c src/gui/textinput.cpp -o src/gui/textinput.o $(CXXFLAGS)

src/gui/timeline.o: src/gui/timeline.cpp
	$(CPP) -c src/gui/timeline.cpp -o src/gui/timeline.o $(CXXFLAGS)

src/gui/view.o: src/gui/view.cpp
	$(CPP) -c src/gui/view.cpp -o src/gui/view.o $(CXXFLAGS)
