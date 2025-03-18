#include "SessionManager.hpp"
#include "color.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>

unsigned int additionalEntropy = static_cast<unsigned int>(reinterpret_cast<size_t>(&additionalEntropy) & 0xFFFFFFFF);

SessionManager::SessionManager()
{
	// init random seed only once
	srand(time(NULL) ^ additionalEntropy);
}

SessionManager::~SessionManager() {}

std::string SessionManager::generateSessionID()
{
	static const char alphanum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

	// append 32 random char to sessionID
	std::string sessionID;
	for (int i = 0; i < 32; ++i)
		sessionID += alphanum[rand() % (sizeof(alphanum) - 1)];

	return sessionID;
}

std::string SessionManager::createSession(int duration)
{
	// generate a unique session ID
	std::string sessionID = generateSessionID();

	// create session data with expiry time
	SessionData data;
	std::time_t now = std::time(NULL);
	data.expiry = now + duration;
	data.creationTime = now;

	// store the session data
	sessions[sessionID] = data;

	return sessionID;
}

bool SessionManager::validateSession(const std::string &sessionID)
{
	if (sessionID.empty())
		return false;

	std::map<std::string, SessionData>::iterator it = sessions.find(sessionID);

	if (it != sessions.end())
	{
		std::time_t now = std::time(NULL);

		// check if the session is still valid based on its expiry time

		if (now < it->second.expiry)
			return true;
		else
			sessions.erase(it); // clean up expired session
	}
	return false;
}

size_t SessionManager::getActiveSessionCount() const
{
	return sessions.size();
}
