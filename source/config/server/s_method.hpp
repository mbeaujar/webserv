#ifndef _S_METHOD_HPP_
# define _S_METHOD_HPP_

# define TIME 1000

struct s_method {
	s_method();
	s_method(s_method const &);
	s_method& operator=(s_method const & rhs);

    bool m_get;
    bool m_post;
    bool m_delete;
};

#endif /* _S_METHOD_HPP_ */