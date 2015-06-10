CC=g++
VPATH=src
OBJECTS=app.o String.o vector.o

app: $(OBJECTS)
	$(CC) $(OBJECTS) -o
app.o: app.cpp
	$(CC) app.cpp -c
String.o: String.h String.cpp
	$(CC) String.h String.cpp -c
vector.o: vector.h
	$(CC) vector.h -c

clean:
	rm *.h.gch *.o
