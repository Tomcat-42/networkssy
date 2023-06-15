#include <networkssy/error.hpp>

#include <cerrno>
#include <string>
#include <unordered_map>

namespace networkssy::error {

const std::unordered_map<int, std::string> ERROR_MAP = {
  {EACCES,
   "Permission to create a socket of the specified "
   "type and/or protocol is denied."},
  {EAFNOSUPPORT,
   "The implementation does not support the "
   "specified address family."},
  {EINVAL,
   "Unknown protocol, or protocol family not "
   "available."},
  {EMFILE,
   "The per-process limit on the number of open "
   "file descriptors has been reached."},
  {ENOBUFS,
   "Insufficient resources were available in the "
   "system to perform the operation."},
  {ENOMEM,
   "Insufficient memory was available to fulfill "
   "the request."},
  {EPROTONOSUPPORT,
   "The protocol type or the specified "
   "protocol is not supported within "
   "this domain."},
  {EPROTOTYPE,
   "The socket type is not supported by the "
   "protocol."},
  {INVALID_SOCKETFD, "The socket file descriptor is invalid."},
};
} // namespace networkssy::error
