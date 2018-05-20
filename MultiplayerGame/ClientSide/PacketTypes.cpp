#include "PacketTypes.h"
#include <iostream>

void StampPacket(const PacketType& l_type, sf::Packet& l_packet)
{
	l_packet << PacketID(l_type);
}

void CreateMovePacket(sf::Packet& p, float x, float y)
{

	StampPacket(PacketType::Player_Move, p);

	p << x << y;
	
}

void CreateFirePacket(sf::Packet & p, sf::Vector2f pos, sf::Vector2f dir, const int& bulletOfTeam)
{
	StampPacket(PacketType::Player_Fire, p);

	p << pos.x << pos.y << dir.x << dir.y << bulletOfTeam;
}
