Defined in Node.h
Used by [Node_Link][]
Used by [NodeRBTree][] inside of [NodeStore_pvt][] and others

### Node_Two

- uint32_t reach_pvt;
  - The reach of the node (how big/fast/close it is).
  - Since reach is a fraction, the reach number represents a percentage where 0xFFFFFFFF = 100%
  - DO NOT ALTER THIS OUTSIDE OF NODESTORE

- uint64_t timeLastPinged;
  - Time the node was last pinged
  - **not** reset on path changes.
- int marked;
  - This is only used to mark/sweep nodes in getWorstNode(), it is meaningless otherwise.

- struct [Address][] address;
  - addr of current node

- struct [EncodingScheme][]* encodingScheme;
  - The encoding method used by this node.
    - TODO: list encodeing methods here

- struct [PeerRBTree][] { struct Node_Link* rbh_root; } peerTree;
  - Peers of this node for which we know the forward direction.
  - Use RB_NFIND(PeerRBTree, node->peerTree, struct type* elm)

- struct [Node_Link][]* reversePeers;
  - Used for freeing the links associated with this node.


- struct [Node_Link][]* bestParent_pvt;
  - The best link for getting to this node.

- struct { struct [Node_Two][]* rbe_left; struct [Node_Two][]* rbe_right; struct [Node_Two][]* rbe_parent; int rbe_color; } nodeTree;
  - Used by nodeStore's RBTree of nodes by address.

- struct [Allocator][]* alloc;
  - TODO: find out what this does

- Identity
  - Magical macro mostly a noop  

[Node_Link]: ./Node_link.md
[NodeStore_pvt]: ./NodeStore_pvt.md
[Node_Two]: ./Node_Two.md
TODO: finish these links
[NodeRBTree]: ./NodeRBTree.md
[EncodingScheme]: ./EncodingScheme.md
[Address]: ./Address.md
[PeerRBTree]: ./PeerRBTree.md
