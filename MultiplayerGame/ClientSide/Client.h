#pragma once
#include <functional>
#include "PacketTypes.h"
#include "NetworkDefinitions.h"
#include <iostream>

#define CONNECT_TIMEOUT 10000

class Client;
using PacketHandler = std::function<void(const PacketID&, sf::Packet&, Client*)>;


class Client
{
public:
	Client(std::string playerName, int teamIndex);
	~Client();

	bool Connect();
	bool Disconnect();
	void Listen();
	bool Send(sf::Packet& l_packet);
	const sf::Time& GetTime() const;
	const sf::Time& GetLastHeartbeat() const;
	void SetTime(const sf::Time& l_time);
	void SetServerInformation(const sf::IpAddress& l_ip, const PortNumber& l_port);

	template<class T>
	void Setup(void(T::*l_handler)(const PacketID&, sf::Packet&, Client*), T* l_instance)
	{
		m_packetHandler = std::bind(l_handler, l_instance, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	}

	void Setup(void(*l_handler)(const PacketID&, sf::Packet&, Client*));

	void UnregisterPacketHandler();

	void Update(const sf::Time& l_time);
	bool isConnected() const;

	void SetPlayerName(const std::string& l_name);
	sf::Mutex& GetMutex();

	int GetClientID() const;
	

private:
	std::string m_playerName;
	int m_teamIndex;

	int id_client;

	sf::UdpSocket m_socket;
	sf::IpAddress m_serverIp;
	PortNumber m_serverPort;
	PacketHandler m_packetHandler;
	bool m_connected;
	sf::Time m_serverTime;
	sf::Time m_lastHeartbeat;

	sf::Thread m_listenThread;
	sf::Mutex m_mutex;

};

