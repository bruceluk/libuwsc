/*
 * Copyright (C) 2017 Jianhui Zhao <jianhuizhao329@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
 * USA
 */

#ifndef _LIB_UWSC_H
#define _LIB_UWSC_H

#include <ev.h>

enum {
    UWSC_CLOSE_STATUS_NORMAL                = 1000,
    UWSC_CLOSE_STATUS_GOINGAWAY             = 1001,
    UWSC_CLOSE_STATUS_PROTOCOL_ERR          = 1002,
    UWSC_CLOSE_STATUS_UNACCEPTABLE_OPCODE   = 1003,
    UWSC_CLOSE_STATUS_RESERVED              = 1004,
    UWSC_CLOSE_STATUS_NO_STATUS             = 1005,
    UWSC_CLOSE_STATUS_ABNORMAL_CLOSE        = 1006,
    UWSC_CLOSE_STATUS_INVALID_PAYLOAD       = 1007,
    UWSC_CLOSE_STATUS_POLICY_VIOLATION      = 1008,
    UWSC_CLOSE_STATUS_MESSAGE_TOO_LARGE     = 1009,
    UWSC_CLOSE_STATUS_EXTENSION_REQUIRED    = 1010,
    UWSC_CLOSE_STATUS_UNEXPECTED_CONDITION  = 1011,
    UWSC_CLOSE_STATUS_TLS_FAILURE           = 1015
};

enum {
    UWSC_ERROR_IO = 1,
    UWSC_ERROR_INVALID_HEADER,
    UWSC_ERROR_SERVER_MASKED,
    UWSC_ERROR_NOT_SUPPORT,
    UWSC_ERROR_PING_TIMEOUT,
    UWSC_ERROR_CONNECT,
    UWSC_ERROR_SSL_HANDSHAKE,
    UWSC_ERROR_PARSE_FRAME
};

enum {
    CLIENT_STATE_CONNECTING,
    CLIENT_STATE_SSL_HANDSHAKE,
    CLIENT_STATE_HANDSHAKE,
    CLIENT_STATE_PARSE_MSG_HEAD,
    CLIENT_STATE_PARSE_MSG_PAYLEN,
    CLIENT_STATE_PARSE_MSG_PAYLOAD
};

enum {
    UWSC_OP_CONTINUE   = 0x0,
    UWSC_OP_TEXT       = 0x1,
    UWSC_OP_BINARY     = 0x2,
    UWSC_OP_CLOSE      = 0x8,
    UWSC_OP_PING       = 0x9,
    UWSC_OP_PONG       = 0xA
};

typedef struct uwsc_client uwsc_client_t;
typedef void (*uwsc_onopen)(uwsc_client_t *cl, void* context);
typedef void (*uwsc_onmessage)(uwsc_client_t *cl, void* context, void *data, size_t len, uint8_t opcode, bool isfinal);
typedef void (*uwsc_onerror)(uwsc_client_t *cl, void* context, int err, const char *msg);
typedef void (*uwsc_onclose)(uwsc_client_t *cl, void* context, int code, const char *reason);

uwsc_client_t* uwsc_new(struct ev_loop *loop, const char *url,
    int ping_interval, const char *extra_header);
uwsc_client_t* uwsc_new_ex(struct ev_loop *loop, const char* remote_host, 
    const char* host, int port, 
    const char* path, bool ssl,
    int ping_interval, const char *extra_header);
void uwsc_init_cb(uwsc_client_t* client, void* context, 
    uwsc_onopen opopen, uwsc_onmessage opmessage, uwsc_onerror operror, uwsc_onclose opclose);
int uwsc_send(uwsc_client_t *cl, const void *data, size_t len, int op);
int uwsc_send_ex(struct uwsc_client *cl, int op, int num, ...);
int uwsc_send_close(struct uwsc_client *cl, int code, const char *reason);
void uwsc_ping(struct uwsc_client *cl);
void uwsc_close(uwsc_client_t* client);

#endif
