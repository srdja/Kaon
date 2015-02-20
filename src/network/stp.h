/*
 * gstp.h
 *
 *  Created on: Nov 27, 2014
 *      Author: srdja
 */

#ifndef __GSTP_H__
#define __GSTP_H__

#include <stdbool.h>
#include <stdint.h>
#include <SDL2/SDL_net.h>

typedef struct stp_channel_out_s {
    bool      is_recv_channel;

    uint32_t  remote_address;
    uint16_t  port;
    IPaddress ip;

    UDPsocket socket;
    UDPpacket last_packet;
    uint32_t  packet_sequence;

} STPChannel;

typedef struct stp_packet_s {
    int   adress;
    int   port;
    char *msg;


} STPPacket;

STPChannel *stp_channel_open  (const char *addr, uint16_t port);
void        stp_channel_close (STPChannel *stp);
//bool        stp_send          (STPChannel *stp, STPPacket *packet);
//bool        stp_recv          (STPChannel *stp, STPPacket *packet);

#endif /* GSTP_H_ */
