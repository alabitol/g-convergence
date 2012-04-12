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

connection: connection.c response.c
${CC} -c $^

certificate: certificate.c
${CC} -c $^

response: response.c certificate.c
${CC} -c $^

cache: cache.c
${CC} -c $^

clean:
/bin/rm -f ${OBJS} \#*# .#*
/bin/rm -f notary test
