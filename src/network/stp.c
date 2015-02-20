/*
 * gstp.c
 *
 *  Created on: Nov 27, 2014
 *      Author: srdja
 */
// game state transmission protocol

// XXX Console print SDL errors

#include <stdlib.h>

#include "stp.h"

typedef struct stp_header_s {
    uint32_t seq_number;
    uint32_t ack_number;
    uint8_t  ctrl_bits;
} STPHeader;

static void stp_header_write (STPChannel *stp, STPHeader *header);
static void stp_header_read  (STPChannel *stp, STPHeader *header);


/** ----------------------------------------------------------------------------
 * Creates a new STP channel.
 *
 * @param[in] addr the remote address or NULL if this channel is the receiving
 *                 channel.
 * @param[in] port the remote port number of 0 if this channel is the receiving
 *                 channel.
 *
 * @return a new STP channel
 -----------------------------------------------------------------------------*/
STPChannel *
stp_channel_open(const char *addr, uint16_t port)
{
    if ((addr == NULL && port != 0) ||
        (addr != NULL && port == 0)) {
        return NULL;
    }

    STPChannel *stp = calloc(1, sizeof(STPChannel));

    if (addr == NULL && port == NULL)
        stp->is_recv_channel = true;


    SDLNet_ResolveHost(&(stp->ip), addr, port);

    if (!(stp->socket = SDLNet_UDP_Open(port))) {
       free(stp);
       return NULL;
    }

    if (SDLNet_UDP_Bind(stp->socket, -1, &(stp->ip)) == -1) {
        SDLNet_UDP_Close(stp->socket);
        free(stp);
        return NULL;
    }

    return stp;
}

/** ----------------------------------------------------------------------------
 *
 * @param[in] stp
 -----------------------------------------------------------------------------*/
void
stp_channel_close(STPChannel *stp)
{
    SDLNet_UDP_Close(stp->socket);
    free(stp);
}
/*
/** ----------------------------------------------------------------------------
 *
 *
 *
 -----------------------------------------------------------------------------*//*
void
stp_send(STPChannel *stpc, STPMessage *msg, bool reliable)
{

}*/

/** ----------------------------------------------------------------------------
 *
 *
 *
 -----------------------------------------------------------------------------*//*
void
stp_recv(STPChannel *stpc)
{

}*/
