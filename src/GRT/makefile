######################################################################
#                                                                    #
#             makefile for raytrace                                  #
#                                                                    #
######################################################################

CC = g++ -I/usr/local/gcc/include/g++-include 
CC = g++ -I/usr/local/include/g++-include 
CC = g++ -I/usr/local/include/ 
CC = g++ -I/usr/local/include/libio -I/usr/local/include/libstdc++
CC = CC
CC = g++
GETPAR = ../getpar
LIBS = -L${GETPAR}/lib  -lget -lm 

CLASS = ../Classes
GENERAL_H = $(CLASS)/include/

BIN = ../../bin/
F77 = f77 -g


test_gray: \
	test_gray.o  gray.o graypf.o ray.o raypm.o model.o raypath.o \
	ReadRayfile.o findLeftRight.o ReadRayParfile.o
	$(CC) -o $(BIN)test_gray \
	test_gray.o  gray.o graypf.o ray.o raypm.o model.o raypath.o \
	ReadRayfile.o findLeftRight.o ReadRayParfile.o \
	$(LIBS)

findpath: \
	findpath.o  gray.o graypf.o ray.o raypm.o model.o raypath.o \
	ReadRayfile.o findLeftRight.o ReadRayParfile.o
	$(CC) -o $(BIN)findpath \
	findpath.o  gray.o graypf.o ray.o raypm.o model.o raypath.o \
	ReadRayfile.o findLeftRight.o ReadRayParfile.o \
	$(LIBS)

findpath0: \
	findpath0.o  gray.o graypf.o ray.o raypm.o model.o raypath.o \
	ReadRayfile.o findLeftRight.o ReadRayParfile.o
	$(CC) -o $(BIN)findpath0 \
	findpath0.o  gray.o graypf.o ray.o raypm.o model.o raypath.o \
	ReadRayfile.o findLeftRight.o ReadRayParfile.o \
	$(LIBS)

findpath1: \
	findpath1.o  gray.o graypf.o ray.o raypm.o model.o raypath.o \
	ReadRayfile.o findLeftRight.o ReadRayParfile.o
	$(CC) -o $(BIN)findpath1 \
	findpath1.o  gray.o graypf.o ray.o raypm.o model.o raypath.o \
	ReadRayfile.o findLeftRight.o ReadRayParfile.o \
	$(LIBS)

travel_time: \
	travel_time.o  gray.o graypf.o ray.o raypm.o model.o raypath.o \
	ReadRayfile.o
	$(CC) -o $(BIN)travel_time \
	travel_time.o  gray.o graypf.o ray.o raypm.o model.o raypath.o \
	ReadRayfile.o \
	$(LIBS)

findpo: \
	findpo.o  gray.o graypf.o ray.o raypm.o model.o raypath.o \
	ReadRayfile.o findLeftRight.o ReadRayParfile.o
	$(CC) -o $(BIN)findpo \
	findpo.o  gray.o graypf.o ray.o raypm.o model.o raypath.o \
	ReadRayfile.o findLeftRight.o ReadRayParfile.o \
	$(LIBS)

caltimes: \
	caltimes.o  gray.o graypf.o ray.o raypm.o model.o raypath.o \
	ReadRayfile.o findLeftRight.o ReadRayParfile.o
	$(CC) -o $(BIN)findpo \
	caltimes.o  gray.o graypf.o ray.o raypm.o model.o raypath.o \
	ReadRayfile.o findLeftRight.o ReadRayParfile.o \
	$(LIBS)

GRAY  = $(CLASS)/gray/
RAY   = $(CLASS)/Ray/
MODEL = $(CLASS)/Model/
RAYPATH = $(CLASS)/raypath/

test_gray.o: \
	test_gray.cpp $(GRAY)gray.h $(RAY)ray.h $(MODEL)model.h
	$(CC) -c -I${GETPAR}/include -I$(MODEL) -I$(RAYPATH) -I$(GRAY) -I$(RAY)\
	-I$(GENERAL_H) -c test_gray.cpp 

findpath.o: \
	findpath.cpp $(GRAY)gray.h $(RAY)ray.h $(MODEL)model.h
	$(CC) -I${GETPAR}/include -I$(MODEL) -I$(RAYPATH) -I$(GRAY) -I$(RAY)\
	-I$(GENERAL_H) -c findpath.cpp 

caltimes.o: \
	caltimes.cpp $(GRAY)gray.h $(RAY)ray.h $(MODEL)model.h
	$(CC) -I${GETPAR}/include -I$(MODEL) -I$(RAYPATH) -I$(GRAY) -I$(RAY)\
	-I$(GENERAL_H) -c caltimes.cpp

findpo.o: \
	findpo.cpp $(GRAY)gray.h $(RAY)ray.h $(MODEL)model.h
	$(CC) -I${GETPAR}/include -I$(MODEL) -I$(RAYPATH) -I$(GRAY) -I$(RAY)\
	-I$(GENERAL_H) -c findpo.cpp 

findpath0.o: \
	findpath0.cpp $(GRAY)gray.h $(RAY)ray.h $(MODEL)model.h
	$(CC) -I${GETPAR}/include -I$(MODEL) -I$(RAYPATH) -I$(GRAY) -I$(RAY)\
	-I$(GENERAL_H) -c findpath0.cpp 

findpath1.o: \
	findpath1.cpp $(GRAY)gray.h $(RAY)ray.h $(MODEL)model.h
	$(CC) -c  -I${GETPAR}/include -I$(MODEL) -I$(RAYPATH) -I$(GRAY) -I$(RAY)\
	-I$(GENERAL_H) -c findpath1.cpp 


travel_time.o: \
	travel_time.cpp $(GRAY)gray.h $(RAY)ray.h $(MODEL)model.h
	$(CC) -I${GETPAR}/include -I$(MODEL) -I$(RAYPATH) -I$(GRAY) -I$(RAY)\
	-I$(GENERAL_H) -c travel_time.cpp 

ReadRayfile.o: \
	ReadRayfile.cpp $(RAY)ray.h $(GENERAL_H)general.h 	
	$(CC) -I$(RAY) -I$(GENERAL_H) -c \
	ReadRayfile.cpp

findLeftRight.o: \
	findLeftRight.cpp $(GENERAL_H)general.h 	
	$(CC) -I$(GENERAL_H) -c \
	findLeftRight.cpp

ReadRayParfile.o: \
	ReadRayParfile.cpp $(GENERAL_H)general.h 	
	$(CC)  -I$(GENERAL_H) -c \
	ReadRayParfile.cpp

# following for gay class
gray.o: \
	$(GRAY)gray.cpp $(GRAY)gray.h $(RAY)ray.h $(MODEL)model.h \
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

#######################################################################
