/**
 * Create an error in response to a query message.
 * 
 */
void DHTTools_craftErrorResponse(const bobj_t* requestMessageDict,
                                 bobj_t* responseMessageDict,
                                 uint64_t errorCode,
                                 const char* errorMessage);

void DHTTools_craftWrongTokenErrorResponse(const Dict* requestMessage,
                                           Dict* responseMessage,
                                           const struct MemAllocator* messageAllocator);

/**
 * Generate a token.
 * Outputs will be 8 character bencoded strings.
 *
 * @param target the target or info hash which is to be announced.
 * @param nodeId the id of the searching node.
 * @param announceAddress if the node is announcing their IP address then that should be included here,
 *                        if they are not then this should be NULL in order to minize reliance on the IP
 *                        network for confidence. Knowing the node id and target of a recent request is
 *                        sufficiently hard that there is no need to introduce added complexity by including
 *                        the ip address in tokens which do not need it. This is only here to prevent replay
 *                        attacks with the least amount of effort possible.
 * @param secret some secret random bytes which should be different each time the module is started.
 * @param allocator the means to acquire memory for storing the token.
 * @return a token which can be validated by DHTTools_isTokenValid() for at least 10 minutes.
 */
bobj_t* DHTTools_generateToken(const char target[20],
                               const char nodeId[20],
                               const char announceAddress[18],
                               const char secret[20],
                               const struct MemAllocator* allocator);

/**
 * Validate a token.
 *
 * @param token the token provided be the other peer in the connection.
 * @param target the target or info hash which is being announced.
 * @param nodeId the id of the announcing node.
 * @param announceAddress if the node is announcing their IP address then that should be included here,
 *                        if they are not then this should be NULL, see: DHTTools_generateToken() for more
 *                        information about when this should and should not be used.
 * @param secret some secret random bytes which should be different each time the module is started.
 * @param allocator the means to acquire memory for storing the token.
 * @return 1 if the token is valid and less than 10 minutes old, 0 otherwise.
 */
int32_t DHTTools_isTokenValid(const bobj_t* token,
                              const char target[20],
                              const char nodeId[20],
                              const char announceAddress[18],
                              const char secret[20]);
