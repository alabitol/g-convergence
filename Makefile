# Makes Convergence notary
# created: February 27, 2012
# revised: March 12, 2012

CFLAGS= -Wall -ggdb3
OBJS= connection.o certificate.o response.o cache.o

notary: notary.c ${OBJS}
	${CC} -o $@ $^

test: notary-test.c
	${CC} -o $@ $^

connection.o: connection.c
	${CC} -c -o $@ $^

certificate.o: certificate.c
	${CC} -c -o $@ $^

response.o: response.c
	${CC} -c -o $@ $^

cache.o: cache.c
	${CC} -c -o $@ $^

clean:
	/bin/rm -f ${OBJS} \#*# .#*
