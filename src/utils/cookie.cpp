#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <string>

std::string getExpirationDate(int seconds)
{
	std::time_t now = std::time(NULL);
	now += seconds;
	struct tm *tm_info = std::gmtime(&now);
	char	   buffer[80];
	std::strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", tm_info);
	return std::string(buffer);
}
