#include "Port.hpp"

bool Port::operator==(Port const & rhs) const { return port == rhs.port && ipv4 == rhs.ipv4; }

bool Port::operator!=(Port const & rhs) const { return !(*this == rhs); }

bool Port::operator<(Port const & rhs) const
{
	if (port != rhs.port)
		return port < rhs.port;
	// egal
	if (ipv4 == rhs.ipv4)
		return false;
	return ipv4 < rhs.ipv4;
}

bool Port::operator<=(Port const & rhs) const { return !(*this > rhs); }

bool Port::operator>(Port const & rhs) const { return rhs < *this; }

bool Port::operator>=(Port const & rhs) const { return !(*this < rhs); }