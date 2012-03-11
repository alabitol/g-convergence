# Makes Convergence notary
# created: February 27, 2012
# revised: March 11, 2012

CFLAGS= -Wall -ggdb3

notary: notary.c connection.o certificate.o response.o
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
	rm -f *.o
