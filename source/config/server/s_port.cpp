#include "s_port.hpp"

s_port::s_port() :
	port(), fd(), ipv4(true) {}

s_port::s_port(s_port const & copy) :
	port(copy.port), fd(copy.fd), ipv4(copy.ipv4) {}

s_port::~s_port() {}

s_port & s_port::operator=(s_port const & copy)
{
	if (this != &copy)
	{
		port = copy.port;
		fd = copy.fd;
		ipv4 = copy.ipv4;
	}
	return *this;
}

bool s_port::operator==(s_port const & rhs) const { return port == rhs.port && ipv4 == rhs.ipv4; }

bool s_port::operator!=(s_port const & rhs) const { return !(*this == rhs); }

bool s_port::operator<(s_port const & rhs) const
{
	if (port != rhs.port)
		return port < rhs.port;
	// egal
	if (ipv4 == rhs.ipv4)
		return false;
	return ipv4 < rhs.ipv4;
}

bool s_port::operator<=(s_port const & rhs) const { return !(*this > rhs); }

bool s_port::operator>(s_port const & rhs) const { return rhs < *this; }

bool s_port::operator>=(s_port const & rhs) const { return !(*this < rhs); }