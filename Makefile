# Makes Convergence notary
# created: February 27, 2012
# revised: March 12, 2012

CURLFLAG = -lcurl
MHDFLAG = -lmicrohttpd
SSLFLAG = -lcrypto
CFLAGS= -Wall -ggdb3
OBJS= connection.o certificate.o response.o cache.o

notary: notary.c ${OBJS}
	${CC} -o $@ $^ ${CURLFLAG} ${MHDFLAG} ${SSLFLAG}

test: notary-test.c ${OBJS}
	${CC} -o $@ $^ ${CURLFLAG} ${MHDFLAG} ${SSLFLAG} ${CFLAGS}

connection.o: connection.c response.c
	${CC} -c  $^ 

certificate.o: certificate.c
	${CC} -c -o $@ $^

response.o: response.c certificate.c
	${CC} -c -o $@ $^

cache.o: cache.c
	${CC} -c -o $@ $^

clean:
	/bin/rm -f ${OBJS} \#*# .#*
