CXX = g++
CXX_FLAGS = -std=c++17 # important flags.
CXX_FLAGS += -Wuninitialized -Wall -Wextra -Werror -pedantic -Wfatal-errors # source code quality.
CXX_FLAGS += -O3 -ffast-math # "production" version.
#CXX_FLAGS += -O0 -g # debug version.
CXX_FLAGS += -I./ # include paths.

SRCS = k-opt.cpp Tour.cpp \
   LengthMap.cpp point_quadtree/Node.cpp \
   forward/Finder.cpp

%.o: %.cpp; $(CXX) $(CXX_FLAGS) -o $@ -c $<

OBJS = $(SRCS:.cpp=.o)

all: $(OBJS); $(CXX) $^ -o k-opt.out

clean: ; rm -rf k-opt.out $(OBJS) *.dSYM
