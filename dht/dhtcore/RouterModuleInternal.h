/**
 * Internal structures which are needed for testing but should not be exposed to the outside world.
 */

/** The context for this module. */
struct RouterModule
{
    /** This node's address, doesn't contain networkAddress component. */
    struct Address address;

    /** An AverageRoller for calculating the global mean response time. */
    void* gmrtRoller;

    /** The storage for the searches which are in progress. */
    struct SearchStore* searchStore;

    /** The storage for the nodes. */
    struct NodeStore* nodeStore;

    /** The sum of reach of all nodes. */
    uint64_t totalReach;

    /** The registry which is needed so that we can send messages. */
    struct DHTModuleRegistry* registry;

    /** The libevent event base for handling timeouts. */
    struct event_base* eventBase;

    struct Janitor* janitor;
};
