#include "entity.h"

void Sync_s::set_socket(SOCKET sock)
{
    s_sock = sock;
}

void Sync_s::begin(sockaddr_in* addr)
{
    s_address = addr;
}

/* Must be called after enqueing data for a client */
void Sync_s::end()
{
    if (s_num_ents > 0)
    {
        dispatch();
    }
}

void Sync_s::dispatch()
{
    /* Share the same buffer for all entities */
    static char buf[MAX_BUF_SIZE];

    memcpy(buf, &s_num_ents, 0);
    memcpy(buf + sizeof(s_num_ents), s_head_buf, s_head_len);
    memcpy(buf + sizeof(s_num_ents) + s_head_len, s_data_buf, s_data_len);
    sendto(s_sock, buf, sizeof(s_num_ents) + s_data_len + s_head_len, 0, (sockaddr*) s_address, sizeof(sockaddr));
}

void Sync_s::enqueue()
{
    /* Share the same buffer for all entities */
    static char buf[MAX_BUF_SIZE];

    EClass eclass = this->get_eclass();

    int len = this->fill_buffer(buf);

    uint16_t packet_size = (sizeof(eclass) + s_head_len) + (s_data_len + len) + sizeof(s_num_ents);

    if (packet_size > MAX_PACKET_SIZE)
    {
        dispatch();
        s_head_len = 0;
        s_data_len = 0;
        s_num_ents = 0;
    }

    memcpy(s_head_buf + s_head_len, &eclass, sizeof(eclass));
    s_head_len += sizeof(eclass);

    memcpy(s_data_buf + s_data_len, buf, len);
    s_data_len += len;

    s_num_ents++;
}

int Monster_e::fill_buffer(char* buf)
{
    int len = sizeof(this->pos.pos);
    memcpy(buf, this->pos.pos, len);
    return len;
}

int Hero_e::fill_buffer(char* buf)
{
    int len = sizeof(this->pos.pos);
    memcpy(buf, this->pos.pos, len);
    return len;
}

int DroppedItem_e::fill_buffer(char* buf)
{
    int len = sizeof(this->pos.pos);
    memcpy(buf, this->pos.pos, len);
    return len;
}

void Monster_e::consume_buffer(char* buf)
{
    memcpy(this->pos.pos, buf, sizeof(this->pos.pos));
}

void Hero_e::consume_buffer(char* buf)
{
    memcpy(this->pos.pos, buf, sizeof(this->pos.pos));
}

void DroppedItem_e::consume_buffer(char* buf)
{
    memcpy(this->pos.pos, buf, sizeof(this->pos.pos));
}

EClass Hero_e::get_eclass()
{
    return ECLASS_HERO;
}

EClass Monster_e::get_eclass()
{
    return ECLASS_MONSTER;
}

EClass DroppedItem_e::get_eclass()
{
    return ECLASS_DROPPED_ITEM;
}

SOCKET Sync_s::s_sock = INVALID_SOCKET;
sockaddr_in* Sync_s:: s_address = NULL;

char Sync_s::s_data_buf[MAX_BUF_SIZE] = { 0 };
uint16_t Sync_s::s_data_len = 0;

char Sync_s::s_head_buf[MAX_BUF_SIZE] = { 0 };
uint16_t Sync_s::s_head_len = 0;

uint8_t Sync_s::s_num_ents = 0;


void Hero_e::tick(double dt)
{

}

void Hero_e::render(vec3 pos, vec3 origin) const
{
    
}