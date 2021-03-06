// ****************************************************************************************
//
// GameEngine of the University of Augsburg
// --------------------------------------
// Copyright (C) 2007 Volker Wiendl
//
// This file is part of the GameEngine developed at the
// Lab for Multimedia Concepts and Applications of the University of Augsburg
//
// This software is distributed under the terms of the Eclipse Public License v1.0.
// A copy of the license may be obtained at: http://www.eclipse.org/legal/epl-v10.html
//
// *************************************************************************************************
//


// ****************************************************************************************
//
// GameEngine Core Library of the University of Augsburg
// ---------------------------------------------------------
// Copyright (C) 2011 Christoph Nuscheler
//
// ****************************************************************************************
#ifndef GAMENETWORKMANAGER_H_
#define GAMENETWORKMANAGER_H_

#include "GameEngine.h"

// make sure to link against Winsock2
#if defined PLATFORM_WIN
#	pragma comment( lib, "ws2_32.lib" )
#else
#	include <sys/socket.h>
#	include <netinet/in.h>
	typedef int SOCKET;
	typedef struct sockaddr_in SOCKADDR_IN;
	typedef struct sockaddr SOCKADDR;
#endif

#include "GameEngine_Network.h"

#include <set>
#include <map>
#include <string>

struct ClientRecord;
struct NetworkMessage;

class GameComponent;
class GameState;

/** \addtogroup GameEngineCore
 * @{
 */

class DLLEXP GameNetworkManager
{
	friend class GameModules;

public:

	GameEngine::Network::NetworkState getState();

	bool init();

	void update();

	void release();

	void setupServer();

	void connectToServer(const char* ip_addr);

	void disconnect();

	bool removeClient(const unsigned long clientID);

	// registers a GameComponent on the server for transmitting its state to clients
	bool registerServerComponent(GameComponent* component);
	bool deregisterServerComponent(GameComponent* component);

	void requestClientUpdate(unsigned long clientID, const char* entityID, const char* componentID);
	void disrequestClientUpdate(unsigned long clientID, const char* entityID, const char* componentID);

	// register callbacks
	void registerCallbackOnClientConnect(void (*callback)(unsigned long));      // long holds at least 4 bytes
	void registerCallbackOnClientDisconnect(void (*callback)(unsigned long));
	void registerCallbackOnStateRequest(void (*callback)(std::string, std::string));

	bool setOption(GameEngine::Network::NetworkOption option, const unsigned long value);
	bool setOption(GameEngine::Network::NetworkOption option, const char* value);
	bool setOption(GameEngine::Network::NetworkOption option, const bool value);

private:
	/// Private Contstructor ( friend class GameModules )
	GameNetworkManager();
	/// Private Destructor (friend class GameModules )
	~GameNetworkManager();

	// current state of the network manager
	GameEngine::Network::NetworkState	m_currentState;

	// UDP socket
	SOCKET			m_socket;

	// GameState object for reading/setting components' states
	GameState*		m_gamestate;



	// receives / handles clients' requests
	void sv_receiveMessages();
	void sv_handleIncomingMessage(SOCKADDR_IN* fromaddr, SOCKET fromsocket);

	// collects GameComponents' states and sends them to clients
	void sv_transmitComponentStates();

	// adds a new client to the server's list
	uint32_t sv_addClient(SOCKADDR_IN client);
	uint32_t sv_addClient(SOCKET clientsocket);

	// removes a client from the server's list
	bool sv_removeClient(uint32_t clientID);

	// sends the outgoing NetworkMessage to a certain client
	void sv_sendOutgoingMessageTo(ClientRecord* client);
	void sv_sendOutgoingMessageTo(uint32_t clientID);

	// broadcasts the outgoing NetworkMessage to all clients
	void sv_broadcastOutgoingMessage();

	// test for timed out clients
	void sv_testClientsTimeout();

	// send requests to clients
	void sv_sendStateRequests();

	// accepts incoming TCP connections (doesn't accept the client itself)
	void sv_acceptIncomingTCPConnections();



	// sends a connect message to server
	void cl_connectToServer();

	void cl_awaitAccept();

	void cl_handleServerMessages();

	void cl_transmitComponentStates();

	void cl_disconnect();



    // reads one message from the TCP stream, if possible
    bool readNextTCPMessage(NetworkMessage* network_message, SOCKET tcp_socket);



	// server vars
	uint32_t			m_sv_tick;
	uint32_t			m_sv_tickinterval;
	SOCKADDR_IN		    m_sv_adress;
	uint32_t			m_sv_clientid;
	uint32_t			m_sv_clientTimeoutInterval;
	std::set<SOCKET>				    m_sv_orphanSockets;
	std::map<uint32_t,ClientRecord*>	m_sv_clients;
	std::set<GameComponent*>		    m_sv_components;



	// client vars
	uint32_t			m_cl_id;
	uint32_t			m_cl_tick;
	uint32_t			m_cl_tickinterval;
	SOCKADDR_IN		m_cl_serveradress;
	uint32_t			m_cl_retrytimer;
	std::set<GameComponent*>	m_cl_components;




	// common vars
	char*			m_applicationID;
	bool			m_useCompression;
	bool			m_useTCP;



	// network buffers
	NetworkMessage*	m_outgoing_message;
	NetworkMessage*	m_incoming_message;

	char* m_sendBuffer;
	char* m_receiveBuffer;



	// event callbacks
	void			(*m_onClientConnect)(unsigned long);
	void			(*m_onClientDisconnect)(unsigned long);

	void			(*m_onStateRequest)(std::string, std::string);
};

/*! @}*/
#endif
