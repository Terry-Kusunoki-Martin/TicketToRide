CXX = clang++
CXXFLAGS = -g -Wall

# Add any new .h and -inl.h files to the INCLUDES definition below:
INCLUDES = library/list.h library/queue.h library/dictionary.h \
	   library/priorityQueue.h library/pair.h \
	   library/arrayQueue.h library/arrayQueue-inl.h \
	   library/circularArrayList.h library/circularArrayList-inl.h \
	   library/hashTable.h library/hashTable-inl.h \
	   library/binaryHeap.h library/binaryHeap-inl.h \
	   edge.h edge-inl.h graph.h graph-inl.h ugraph.h ugraph-inl.h \
	   graph-algorithms.h graph-algorithms-inl.h #ticket.h




# If you create any new non-templated classes, add an appropriate .o file
# to the OBJECTS definition below:
OBJECTS = ticket.o


# If you create a new test program, add the name of the test program
# to the PROGRAMS definition below and then create a rule for that program
PROGRAMS = testGraph ticketToRide

all: $(PROGRAMS)

testGraph: $(INCLUDES) $(OBJECTS) testGraph.cpp
	$(CXX) $(CXXFLAGS) -o $@ $@.cpp $(OBJECTS)

ticketToRide: $(INCLUDES) $(OBJECTS) ticketToRide.cpp
	$(CXX) $(CXXFLAGS) -o $@ $@.cpp $(OBJECTS)

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $<
clean:
	rm -f *.o $(PROGRAMS)

.PHONY: all clean
