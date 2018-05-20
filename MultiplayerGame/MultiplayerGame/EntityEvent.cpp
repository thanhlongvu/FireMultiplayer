#include "EntityEvent.h"

void PlayerStatus(const int id_player, Player* l_player, sf::Packet & l_packet)
{
	l_packet << id_player << l_player->getHeart() << l_player->getPosition().x << l_player->getPosition().y << l_player->GetTeamIndex() << l_player->GetPlayerName();
}
