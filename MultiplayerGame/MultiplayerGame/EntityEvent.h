#pragma once
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include "Player.h"
enum class EntityEvent
{
	Spawned, Despawned, Colliding_X, Colliding_Y, Move, Attack
};

void PlayerStatus(const int id_player, Player* l_player, sf::Packet& l_packet);
