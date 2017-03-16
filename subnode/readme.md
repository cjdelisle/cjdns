# Cjdns subnode

This is the part which communicates with the cjdns supernode. It fulfills the Pathfinder protocol
internally to cjdns and it answers the questions asked to it by querying its supernode(s).

In order to discover its supernode(s), it will send findNode (`fn`) queries to its peers.
Any findNode queries which are sent to this node will be forwarded to its supernode and the answers
forwarded back.
