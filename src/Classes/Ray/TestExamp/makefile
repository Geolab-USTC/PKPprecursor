######################################################################
#                                                                    #
#             makefile for raytrace                                  #
#                                                                    #
######################################################################

CC = CC -g 
LIBS = -L/opt/getpar/lib -R/opt/getpar/lib -lget -lm 

CLASS = /home/u1/wen/Classes/

BIN = ./
F77 = f77 -g

all:	test_Model clean 

test_Ray: \
	test_Ray.o  ray.o  raypm.o 
	$(CC) -o $(BIN)test_Ray \
	test_Ray.o  ray.o  raypm.o \
	$(LIBS)

MODEL = $(CLASS)/Model/
RAY   = $(CLASS)/Ray/

test_Ray.o: \
	test_Ray.cpp $(RAY)ray.h
	$(CC) -I/opt/getpar/include -I$(RAY) \
	-c test_Ray.cpp 


# following for gray class
gray.o: \
	$(GRAY)gray.cpp $(GRAY)gray.h $(RAY)ray.h $(Model)model.h \
	$(RAYPATH)raypath.h
	$(CC) -I$(MODEL) -I$(GRAY) -I$(RAY) -I$(RAYPATH) -c \
	$(GRAY)gray.cpp 

graypf.o: \
	$(GRAY)graypf.cpp $(GRAY)gray.h $(RAY)ray.h $(MODEL)model.h
	$(CC) -I$(MODEL) -I$(GRAY) -I$(RAY) -I$(RAYPATH) -c \
	$(GRAY)graypf.cpp 

# following for Ray class
ray.o: \
	$(RAY)ray.cpp $(RAY)ray.h	
	$(CC) -I$(RAY) -c $(RAY)ray.cpp 

raypm.o: \
	$(RAY)raypm.cpp $(RAY)ray.h	
	$(CC) -I$(RAY) -c $(RAY)raypm.cpp 

#following for Model class
model.o: \
	$(MODEL)model.cpp $(MODEL)model.h	
	$(CC) -I$(MODEL) -c $(MODEL)model.cpp 

#following for raypath class
raypath.o: \
	$(RAYPATH)raypath.cpp $(RAYPATH)raypath.h	
	$(CC) -I$(RAYPATH) -c $(RAYPATH)raypath.cpp 

clean:
	rm -f \
	*.o

########################################################################
