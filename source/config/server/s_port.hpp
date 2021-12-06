#ifndef _PORT_HPP_
# define _PORT_HPP_

struct s_port
{
	int port;
	int fd;
	bool ipv4;

	bool operator==(s_port const & rhs) const;
	bool operator!=(s_port const &rhs) const;
	bool operator<(s_port const & rhs) const;
	bool operator<=(s_port const & rhs) const;
	bool operator>(s_port const & rhs) const;
	bool operator>=(s_port const & rhs) const;
};

#endif /* _PORT_HPP_ */