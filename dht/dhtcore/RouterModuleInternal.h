/**
 * Internal structures which are needed for testing but should not be exposed to the outside world.
 */

/** The context for this module. */
struct RouterModule
{
    /** A bencoded string with this node's address tag. */
    String* myAddress;

    /** An AverageRoller for calculating the global mean response time. */
    void* gmrtRoller;

    struct SearchStore* searchStore;

    struct NodeStore* nodeStore;

    /** The registry which is needed so that we can send messages. */
    struct DHTModuleRegistry* registry;

    /** The libevent event base for handling timeouts. */
    struct event_base* eventBase;
};
