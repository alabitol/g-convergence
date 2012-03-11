# Makes Convergence notary
# created: February 27, 2012
# revised: March 11, 2012

CFLAGS= -Wall -ggdb3

notary: notary.c connection.o certificate.o response.o
	${CC} -o $@ $^

test: notary-test.c
	${CC} -o $@ $^

clean:
	rm -f *.o
