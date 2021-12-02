#ifndef _PORT_HPP_
# define _PORT_HPP_

struct Port
{
	int port;
	bool ipv4;

	bool operator==(Port const & rhs) const;
	bool operator!=(Port const &rhs) const;
	bool operator<(Port const & rhs) const;
	bool operator<=(Port const & rhs) const;
	bool operator>(Port const & rhs) const;
	bool operator>=(Port const & rhs) const;
};

#endif /* _PORT_HPP_ */