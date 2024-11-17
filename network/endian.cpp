#include "network/endian.hpp"

namespace ft {

#if defined(htonll) && defined(ntohll)

uint64_t htonll(uint64_t x) { return ::htonll(x); }
uint64_t ntohll(uint64_t x) { return ::ntohll(x); }

#elif __BYTE_ORDER__ == __BIG_ENDIAN
uint64_t htonll(uint64_t x) { return x; }
uint64_t ntohll(uint64_t x) { return x; }

#elif defined(_DEFAULT_SOURCE) || defined(_BSD_SOURCE)

#include <endian.h>
uint64_t htonll(uint64_t x) { return htobe64(x); }
uint64_t ntohll(uint64_t x) { return be64toh(x); }

#else

#include <arpa/inet.h>
uint64_t htonll(uint64_t x) {
    return ((uint64_t)htonl((x) & 0xFFFFFFFF) << 32) | htonl((x) >> 32);
}
uint64_t ntohll(uint64_t x) {
    return ((uint64_t)ntohl((x) & 0xFFFFFFFF) << 32) | ntohl((x) >> 32);
}

#endif

} /* namespace ft */
