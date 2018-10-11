CXX=g++
CPPFLAGS=-std=c++11 -O3

SRCS=airship_optimizer.cpp battle.cpp airship.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: airship_optimizer

airship_optimizer: $(OBJS)
	$(CXX) $(CPPFLAGS) -o airship_optimizer $(OBJS)

airship_optimizer.o: airship_optimizer.cpp battle.h airship.h
	$(CXX) $(CPPFLAGS) -c airship_optimizer.cpp

battle.o: battle.cpp battle.h airship.h
	$(CXX) $(CPPFLAGS) -c battle.cpp

airship.o: airship.cpp airship.h
	$(CXX) $(CPPFLAGS) -c airship.cpp

clean:
	rm -rf *.o
