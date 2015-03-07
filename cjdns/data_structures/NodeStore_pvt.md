### NodeStore_pvt

- struct [NodeStore][] pub
- struct [Node_Link][]* selfLink
- struct [NodeRBTree][] { struct [Node_Two][]* rbh_root; } nodeTree;
  - inplace struct [NodeRBTree][] generated here  
    it contains a tree holding all nodes orderd by ipv6 address?
- struct [Allocator][]* alloc;
- struct [Node_Link][]* linksToFree;
  - operated on by freePendingLinks()
- struct [RumorMill][]* renumberMill;
  - nodes that have probably been reset?
- struct [Log][]* logger;
  - [sic] The means for this node store to log.
- struct [EventBase][]* eventBase;
  - To track time?
- [Identity][]
  - mesterious checking macro
[NodeStore]: ./NodeStore.md
[Node_Link]: ./Node_Link.md
[Node_Two]: ./Node_Two.md
[NodeRBTree]: ./NodeRBTree.md
[Allocator]: ./Allocator.md
[RumorMill]: ./RumorMill.md
[Log]: ./Log.md
[EventBase]: ./EventBase.md
[Identity]: ./Identity.md
