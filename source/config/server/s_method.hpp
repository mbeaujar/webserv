#ifndef __S_METHOD_HPP__
# define __S_METHOD_HPP__

struct s_method {
	s_method();
	s_method(s_method const &);
	s_method& operator=(s_method const & rhs);

    bool m_get;
    bool m_post;
    bool m_delete;
};

#endif