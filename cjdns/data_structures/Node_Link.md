defined in Node.h
Used by NodeStore_pvt
### Node_Link

- struct {
    struct [Node_Link][]* rbe_left;
    struct [Node_Link][]* rbe_right;
    struct [Node_Link][]* rbe_parent;
    int rbe_color;
} peerTree;
  - Used by the parent's [RBTree][] of links (?).

- int inverseLinkEncodingFormNumber;
 - The Encoding Form number which is used to represent the first director in the path from  
   child to parent.(?)

- uint32_t linkState;
 - The quality of the link between parent and child,  
   between 0xFFFFFFFF (perfect) and 0 (intolerable).

- uint64_t timeLastSeen;
  - The time this link was last seen carrying traffic. (Currently limited to ping traffic.) 

- struct [Node_Two][]* parent;
  - The parent of this peer, this is where the root of the [RBTree][] is. 
  
- struct [Node_Two][]* child;
  - The child of this link.

- struct [Node_Link][]* nextPeer;
  - linked list
  - The next link which points to the same child.
  - For each child there are many links pointing to it,

- struct [Node_Link][]* nextInSplitList;
  - linked list
  - Used internally by NodeStore for creating a list used for splitting links.

- uint64_t cannonicalLabel;
  - The label which would be used to reach the child from the parent.
  - This label is in a cannonical state and must be altered so that the first Director uses
  - at least as many bits as are required to reach the grandparent from the parent
  - in the reverse direction.

- uint64_t discoveredPath;
  - The path which the incoming packet followed when this node was discovered.

Identity
[Node_Link]: ./Node_Link.md
[Node_Two]: ./Node_Two.md
[RBTree]: ./RBTree.md
