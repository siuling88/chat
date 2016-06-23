##############################################################################################
#                                                                                            #
#                                                                                            #
#      88b           d88              88                       ad88  88  88                  #
#      888b         d888              88                      d8"    ""  88                  #
#      88`8b       d8'88              88                      88         88                  #
#      88 `8b     d8' 88  ,adPPYYba,  88   ,d8   ,adPPYba,  MM88MMM  88  88   ,adPPYba,      #
#      88  `8b   d8'  88  ""     `Y8  88 ,a8"   a8P_____88    88     88  88  a8P_____88      #
#      88   `8b d8'   88  ,adPPPPP88  8888[     8PP"""""""    88     88  88  8PP"""""""      #
#      88    `888'    88  88,    ,88  88`"Yba,  "8b,   ,aa    88     88  88  "8b,   ,aa      #  
#      88     `8'     88  `"8bbdP"Y8  88   `Y8a  `"Ybbd8"'    88     88  88   `"Ybbd8"'      #
#                                                                                            #
#                                                                                            #
##############################################################################################
#                                                                                            #
# CONTENIDO: Makefile para aplicacion "main"                                                 #                                                                                 
#                                                                                            #
##############################################################################################
#######-> A U T O R E S <-######
#                              #
# Johanna Chan        08-10218 #
# Carlos Rodriguez    06-40189 #
#                              #
################################

CC	= gcc  -Wall -ggdb
OBJS    = funciones.o hashtable.o libSockets.o cchat.o 
OBJS1   = funciones.o hashtable.o libSockets.o schat.o
HILO	= -pthread

all:		cchat schat

cchat:	$(OBJS)
		$(CC) $(OBJS) -o cchat $(HILO) 

schat:	$(OBJS1)
		$(CC) $(OBJS1) -o schat $(HILO) 

cchat.o:	cchat.c funciones.h
		$(CC) -c cchat.c $(HILO)
		
schat.o:	schat.c funciones.h
		$(CC) -c schat.c $(HILO)

funciones.o:	funciones.c funciones.h
		$(CC) -c funciones.c $(HILO)

libSockets.o:	libSockets.c libSockets.h hashtable.h
		$(CC) -c libSockets.c $(HILO)

hashtable.o:	hashtable.c hashtable.h
		$(CC) -c hashtable.c $(HILO)

clean:
		rm -f ./*.o ./*.c~ ./*.h~ cchat schat Makefile~
