#pragma once
#include <SFML\System.hpp>
#include <SFML\Network.hpp>


using PacketID = sf::Int8;

enum class PacketType
{
	Connect = 0, Disconnect, Heartbeat, Message, List_Player, Player_Disconnect,
	Player_Update, Hurt, Player_Move, Player_Fire, OutOfBounds
};

void StampPacket(const PacketType& l_type, sf::Packet& l_packet);


void CreateMovePacket(sf::Packet&, int, float, float);

void CreateFirePacket(sf::Packet&, int, sf::Vector2f, sf::Vector2f);