#include "world.h"
#include "logging.h"

static int cmp_addr(sockaddr_in* a, sockaddr_in* b)
{
    return (a->sin_addr.s_addr == b->sin_addr.s_addr) && (a->sin_port == b->sin_port);
}

std::string addr_to_string(sockaddr_in& addr)
{
    std::string ip_str = std::string(inet_ntoa(addr.sin_addr));
    std::string port_str = std::to_string(ntohs(addr.sin_port));
    std::string str =  ip_str + ":" + port_str;
    return str;
}

World::World()
{

}

Player::Player(sockaddr_in addr, World& world)
{
    int32_t id = world.m_heroes.new_id();

    /* Init hero here */
    Hero_e hero(id);
    /* -------------- */

    world.m_heroes.set(id, &hero);

    m_addr = addr;
    m_hero_id = id;
    m_world_ptr = &world;
}

Player::~Player()
{
    m_world_ptr->m_heroes.del(m_hero_id);
}

void Player::create(sockaddr_in addr, World& world)
{
    std::string key =  addr_to_string(addr);
    
    if (s_players.find(key) == s_players.end())
    {
        s_players[key] = new Player(addr, world);
        INFO(std::string(key) + " connected");
    }
    else
    {
        INFO(std::string(key) + " tried to connect when already connected");
    }
}

std::unordered_map<std::string, Player*> Player::s_players = { };