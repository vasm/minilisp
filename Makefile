TARGET=minilisp
TARGETDIR=bin
SRCDIR=minilisp
CXXFLAGS=-O2
CXX=g++

all:
	mkdir -p $(TARGETDIR)
	$(CXX) $(CXXFLAGS) $(SRCDIR)/*.cpp -o $(TARGETDIR)/$(TARGET)


