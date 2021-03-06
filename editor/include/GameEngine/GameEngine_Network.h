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
#ifndef GAMEENGINE_NETWORK_H_
#define GAMEENGINE_NETWORK_H_

#include "config.h"
#include <string>

#ifdef MEMORYCHECK
#include <vld.h>
#endif

#include <stdint.h>

/** \addtogroup GameEngineCore
 * @{
 */

namespace GameEngine
{
	/** \addtogroup GameEngineCore
	 *
	 *	The Core module of the GameEngine is responsible for the initialization and update of the
	 *  world as well as for (un-)loading the component plugins and the communication system between
	 *  components.
	 *
 	 * @{
	 */

	struct Network {

		enum NetworkState {
			INVALID_STATE,
			DISCONNECTED,
			CONNECTING_TO_SERVER,
			CONNECTED_TO_SERVER,
			SERVING,
			DISCOVERY
		};

		enum NetworkOption {
			SV_PORT,
			SV_IP,
			SV_TICKRATE,
			SV_CLIENT_TIMEOUT_INTERVAL,
			CL_SERVERPORT,
			CL_TICKRATE,
			USE_COMPRESSION,
			USE_TCP,
			APPLICATION_ID
		};

	};

	/**
	 * Connects the local GameEngine as a client to a remote server
	 * @param ip_addr The server's IPv4 address in dotted decimal form
	 */
	GAMEENGINE_API void connectToServer(const char* ip_addr);

	/**
	 * Disconnects the local GameEngine from the remote server or shuts down local server
	 */
	GAMEENGINE_API void disconnect();

	/**
	 * Sets up the local GameEngine as a server
	 */
	GAMEENGINE_API void startServer();

	/**
	 * Registers a GameComponent on the local server so its gamestate gets transmitted to clients
	 * @return true on success
	 */
	GAMEENGINE_API bool startDistributing(const char* entityID, const char* componentID);

	/**
	 * Removes a GameComponent from the local server
	 * @return true on success
	 */
	GAMEENGINE_API bool stopDistributing(const char* entityID, const char* componentID);

	/**
	 * State updates to this component will be requested from given client.
	 */
	GAMEENGINE_API void requestClientUpdate(unsigned long clientID, const char* entityID, const char* componentID);

	/**
	 * State updates to this component will no longer be requested nor will they be accepted from given client.
	 */
	GAMEENGINE_API void disrequestClientUpdate(unsigned long clientID, const char* entityID, const char* componentID);

	/**
	 * Registers a callback function to be executed when a new client connects
	 */
	GAMEENGINE_API void registerCallbackOnClientConnect(void (*callback)(unsigned long));

	/**
	 * Registers a callback function to be executed when a client disconnects
	 */
	GAMEENGINE_API void registerCallbackOnClientDisconnect(void (*callback)(unsigned long));

	/**
	 * Registers a callback function to be executed when a state is newly requested by the server
	 */
	GAMEENGINE_API void registerCallbackOnStateRequest(void (*callback)(std::string, std::string));

	/**
	 * Returns the current state of the GameNetworkManager
	 * @return the current NetworkState
	 */
	GAMEENGINE_API GameEngine::Network::NetworkState getNetworkState();

	/**
	 * Sets a GameNetworkManager option
	 * @return true on success
	 */
	GAMEENGINE_API bool setNetworkOption(GameEngine::Network::NetworkOption option, const unsigned long value);

	/**
	 * Sets a GameNetworkManager option
	 * @return true on success
	 */
	GAMEENGINE_API bool setNetworkOption(GameEngine::Network::NetworkOption option, const char* value);

	/**
	 * Sets a GameNetworkManager option
	 * @return true on success
	 */
	GAMEENGINE_API bool setNetworkOption(GameEngine::Network::NetworkOption option, const bool value);

	/**
	 * Removes (disconnects) a connected client from the server
	 * @return true on success
	 */
	GAMEENGINE_API bool removeClient(const unsigned long clientID);

	/*! @}*/
}

/*! @}*/

#endif
