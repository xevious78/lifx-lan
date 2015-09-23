#ifndef H_LIFX_LAN_MESSAGES_H
#define H_LIFX_LAN_MESSAGES_H

#include "types.h"

struct lifx_lan_messages
{
    uint8_t seqnum;
};

void lifx_lan_messages_init(struct lifx_lan_messages* m);
void lifx_lan_messages_encode_header(struct lifx_lan_messages* m, struct lifx_lan_header* head, uint16_t message_type, size_t payload_size);
void lifx_lan_messages_encode_get_service(struct lifx_lan_messages* m, struct lifx_lan_header* head);
void lifx_lan_messages_encode_set_power(struct lifx_lan_messages* m, struct lifx_lan_header* head,
    struct lifx_lan_set_power_payload* payload, bool powered_on, uint32_t duration_millis);


void lifx_lan_messages_init(struct lifx_lan_messages* m)
{
    m->seqnum = 0;
}

void lifx_lan_messages_encode_header(struct lifx_lan_messages* m, struct lifx_lan_header* head, uint16_t message_type, size_t payload_size)
{
    memset(head, 0, LIFX_LAN_HEADER_SIZE);
    assert(LIFX_LAN_HEADER_SIZE + payload_size < UINT16_MAX);

    head->size = LIFX_LAN_HEADER_SIZE + payload_size;
    head->protocol = LIFX_LAN_PROTOCOL;
    head->addressable = 1;
    head->source = LIFX_LAN_SOURCE_ID;
    head->sequence = m->seqnum++;
    head->tagged = 1;
    head->type = message_type;
    head->res_required = 1;
}

void lifx_lan_messages_encode_get_service(struct lifx_lan_messages* m, struct lifx_lan_header* head)
{
    lifx_lan_messages_encode_header(m, head, LIFX_LAN_MESSAGE_TYPE_GET_SERVICE, 0);
}

void lifx_lan_messages_encode_get(struct lifx_lan_messages* m, struct lifx_lan_header* head)
{
    lifx_lan_messages_encode_header(m, head, LIFX_LAN_MESSAGE_TYPE_GET, 0);
}

void lifx_lan_messages_encode_set_power(struct lifx_lan_messages* m, struct lifx_lan_header* head,
    struct lifx_lan_set_power_payload* payload, bool powered_on, uint32_t duration_millis)
{
    lifx_lan_messages_encode_header(m, head, LIFX_LAN_MESSAGE_TYPE_SET_POWER, sizeof(*payload));
    payload->level = powered_on ? LIFX_LAN_LEVEL_POWERED_ON : LIFX_LAN_LEVEL_POWERED_OFF;
    payload->duration = duration_millis;
}

#endif