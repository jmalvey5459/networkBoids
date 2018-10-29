#pragma once

// standard includes
#include <stdio.h>
#include <string.h>
#include <string>

// RakNet includes
#include "RakNet/RakPeerInterface.h"
#include "RakNet/MessageIdentifiers.h"
#include "RakNet/RakNetTypes.h"
#include "RakNet/BitStream.h"
#include "RakNet/GetTime.h"

#include "Define.h"
#include "Flock.h"

enum GameMessages
{
	SETMODE_PUSH = ID_USER_PACKET_ENUM,
	SETMODE_SHARE,
	SETMODE_COUPLED,
	DATA_PUSH,
	DATA_SHARE,
	DATA_COUPLED,
	SEND_CLIENTDATA,
	RECIEVE_FLOCK_DATA,
	RECIEVE_FLOCK2_DATA,
	RECIEVE_COUPLEDFLOCK_DATA,
};

#pragma pack(push, 1)
struct ClientData
{
	int ID = SEND_CLIENTDATA;
	Flock clientFlock;
	RakNet::SystemAddress clientIP;
};
#pragma pack(pop)

enum dataMode
{
	PUSH_MODE,
	SHARE_MODE,
	COUPLED_MODE,
};