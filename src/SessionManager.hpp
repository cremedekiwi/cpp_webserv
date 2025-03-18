#pragma once

#include <ctime>
#include <iostream>
#include <map>
#include <string>

struct SessionData
{
	time_t expiry;
	time_t creationTime;
};

class SessionManager
{
  private:
	std::map<std::string, SessionData> sessions;
	std::string						   generateSessionID();

  public:
	SessionManager();
	~SessionManager();

	std::string createSession(int duration);
	bool		validateSession(const std::string &sessionID);
	void		removeSession(const std::string &sessionID);
	void		cleanupExpiredSessions();
	size_t		getActiveSessionCount() const;
};
