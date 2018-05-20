#include "PacketTypes.h"

void StampPacket(const PacketType& l_type, sf::Packet& l_packet)
{
	l_packet << PacketID(l_type);
}

void CreateMovePacket(sf::Packet& p, int id, float x, float y)
{

	StampPacket(PacketType::Player_Move, p);

	p << id << x << y;

}

void CreateFirePacket(sf::Packet & p, int id, sf::Vector2f pos, sf::Vector2f dir, const int& bulletOfTeam)
{
	StampPacket(PacketType::Player_Fire, p);

	p << id << pos.x << pos.y << dir.x << dir.y << bulletOfTeam;
}

void CreateHurtPacket(sf::Packet & p, int id_player, sf::Vector2f pos_explosion)
{
	StampPacket(PacketType::Hurt, p);

	p << id_player << pos_explosion.x << pos_explosion.y;
}
