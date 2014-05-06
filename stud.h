#ifndef stud_h
#define stud_h

#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <getopt.h>
#include <pwd.h>
#include <limits.h>
#include <syslog.h>
#include <stdarg.h>

#include <ctype.h>
#include <sched.h>
#include <signal.h>

#include <openssl/ssl.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/x509.h>
#include <openssl/err.h>
#include <openssl/engine.h>
#include <openssl/asn1.h>
#include <ev.h>

#include "ringbuffer.h"
#include "shctx.h"
#include "configuration.h"

#ifdef PROTO_HTTP
#include "proto_http.h"
#endif

/*
 * Proxied State
 *
 * All state associated with one proxied connection
 */
typedef struct proxystate {
    ringbuffer ring_ssl2clear;          /* Pushing bytes from secure to clear stream */
    ringbuffer ring_clear2ssl;          /* Pushing bytes from clear to secure stream */

    ev_io ev_r_ssl;                     /* Secure stream write event */
    ev_io ev_w_ssl;                     /* Secure stream read event */

    ev_io ev_r_handshake;               /* Secure stream handshake write event */
    ev_io ev_w_handshake;               /* Secure stream handshake read event */

    ev_io ev_w_connect;                 /* Backend connect event */

    ev_io ev_r_clear;                   /* Clear stream write event */
    ev_io ev_w_clear;                   /* Clear stream read event */

    ev_io ev_proxy;                     /* proxy read event */

    int fd_up;                          /* Upstream (client) socket */
    int fd_down;                        /* Downstream (backend) socket */

    int want_shutdown:1;                /* Connection is half-shutdown */
    int handshaked:1;                   /* Initial handshake happened */
    int clear_connected:1;              /* Clear stream is connected  */
    int renegotiation:1;                /* Renegotation is occuring */

    SSL *ssl;                           /* OpenSSL SSL state */

#ifdef PROTO_HTTP
    struct proto_http ph;
#endif

    struct sockaddr_storage remote_ip;  /* Remote ip returned from `accept` */
} proxystate;

#endif
