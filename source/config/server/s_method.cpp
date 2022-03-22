#include "s_method.hpp"

s_method::s_method() : m_get(false), m_post(false), m_delete(false)
{
}

s_method::s_method(s_method const &copy) : m_get(copy.m_get), m_post(copy.m_post), m_delete(copy.m_delete)
{
}

s_method &s_method::operator=(s_method const &rhs)
{
    if (this == &rhs)
        return *this;
    m_get = rhs.m_get;
    m_delete = rhs.m_delete;
    m_post = rhs.m_post;
    return *this;
}
