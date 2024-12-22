INC_DIR:=include/
SRC_DIR:=src/
SRCS:=$(wildcard src/*.cpp)
OBJS:= $(patsubst %.cpp, %.o, $(SRCS))
LIBS:= -lhv -lmysqlcppconn

CXX:=g++

CXXFLAGS:= -w -g  $(addprefix -I , $(INC_DIR))  $(LIBS) 

EXE:=bin/server.exe

$(EXE):$(OBJS)
	$(CXX) -o $(EXE) $(OBJS) $(CXXFLAGS)

clean:
	rm -rf $(EXE)
	rm -rf $(OBJS)
