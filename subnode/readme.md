# Cjdns subnode

This is the part which communicates with the cjdns supernode. It fulfills the Pathfinder protocol
internally to cjdns and it answers the questions asked to it by querying it's supernode(s).

In order to discover it's supernode(s), it will send findNode (`fn`) queries to it's peers.
Any findNode queries which are sent to this node will be forwarded to it's supernode and the answers
forwarded back.
