#ifndef SELECT_HPP
#define SELECT_HPP

namespace ft {

template <unsigned N, typename... Cases>
struct select;

template <unsigned N, typename T, typename... Cases>
struct select<N, T, Cases...> : select<N - 1, Cases...>
{
};

template <typename T, typename... Cases>
struct select<0, T, Cases...>
{
    using type = T;
};

template <unsigned N, typename... Cases>
using Select = typename select<N, Cases...>::type;

} /* namespace ft */

#endif /* SELECT_HPP */
