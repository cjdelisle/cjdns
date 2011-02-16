#include "dht_parser_test_framework.c"

static int findNode_tester(int(*toTest)(const unsigned char *buf, int buflen,
                                        unsigned char *tid_return, int *tid_len,
                                        unsigned char *id_return,
                                        unsigned char *info_hash_return,
                                        unsigned char *target_return,
                                        unsigned short *port_return,
                                        unsigned char *token_return, int *token_len,
                                        unsigned char *nodes_return, int *nodes_len,
                                        unsigned char *nodes6_return, int *nodes6_len,
                                        unsigned char *values_return, int *values_len,
                                        unsigned char *values6_return, int *values6_len,
                                        int *want_return))
{
    int validateQuery(struct dht_parser_input *parserInput)
    {
        char twenty_os[20];
        memset(twenty_os, 0, 20);

        /*printf("parserReturn=%d\n", parserInput->parser_return);*/

        return
            assertTrue((parserInput->tid_len == 2), "transactionId is wrong length\n")
          | assertTrue(0 == memcmp(parserInput->tid_return, "aa", 1),
                       "transactionId is not 'aa'\n")
          | assertTrue(0 == memcmp(parserInput->id_return, "abcdefghij0123456789", 20),
                      "peer's id is incorrect\n")
          | assertTrue(0 == memcmp(parserInput->info_hash_return, twenty_os, 20),
                       "infoHash was set for a find_node!\n")
          | assertTrue(0 == memcmp(parserInput->target_return, "mnopqrstuvwxyz123456", 20),
                       "target ID is incorrect.\n")
          | assertTrue((parserInput->port_return == 0), "port was not set to 0\n")
          | assertTrue((parserInput->token_len == 0), "token length was not 0\n")
          | assertTrue((parserInput->nodes_len == 0), "nodes length was not 0\n")
          | assertTrue((parserInput->nodes6_len == 0), "nodes6 length was not 0\n")
          | assertTrue((parserInput->values_len == 0), "values length was not 0\n")
          | assertTrue((parserInput->values6_len == 0), "values6 length was not 0\n")
          | assertTrue((parserInput->want_return == -1), "want was not -1\n")
          | assertTrue(parserInput->parser_return == FIND_NODE, "query type not find_node\n");
    }
    return generic_test("d1:ad2:id20:abcdefghij01234567896:target20:mnopqrstuvwxyz123456e"
                        "1:q9:find_node1:t2:aa1:y1:qe",
                        toTest, nullModifier, validateQuery);
}

int findNode_test_control(void)
{
    /*printf("testing parse_message()\n");*/
    return findNode_tester(parse_message);
}

int findNode_test(void)
{
    /*printf("testing parse_message1()\n");*/
    return findNode_tester(parse_message1);
}

static int findNode_reply_tester(int(*toTest)(const unsigned char *buf, int buflen,
                                              unsigned char *tid_return, int *tid_len,
                                              unsigned char *id_return,
                                              unsigned char *info_hash_return,
                                              unsigned char *target_return,
                                              unsigned short *port_return,
                                              unsigned char *token_return, int *token_len,
                                              unsigned char *nodes_return, int *nodes_len,
                                              unsigned char *nodes6_return, int *nodes6_len,
                                              unsigned char *values_return, int *values_len,
                                              unsigned char *values6_return, int *values6_len,
                                              int *want_return))
{
    int validateReply(struct dht_parser_input *parserInput)
    {
        char twenty_os[20];
        memset(twenty_os, 0, 20);

        /*printf("nodes reply=%9.9s\n", parserInput->nodes_return);*/

        return
            assertTrue((parserInput->tid_len == 2), "transactionId is wrong length\n")
          | assertTrue(0 == memcmp(parserInput->tid_return, "aa", 1),
                       "transactionId is not 'aa'\n")

          | assertTrue(0 == memcmp(parserInput->id_return, "0123456789abcdefghij", 20),
                      "peer's id is incorrect\n")
          | assertTrue(0 == memcmp(parserInput->info_hash_return, twenty_os, 20),
                       "infoHash was set for a find_node reply.\n")
          | assertTrue(0 == memcmp(parserInput->target_return, twenty_os, 20),
                       "target ID was set for a find_node reply.\n")

          | assertTrue((parserInput->port_return == 0), "port was not set to 0\n")
          | assertTrue((parserInput->token_len == 0), "token length was not 0\n")

          | assertTrue((parserInput->nodes_len == 9), "nodes length was not correct.\n")
          | assertTrue(0 == memcmp(parserInput->nodes_return, "def456...", 9),
                       "find_node reply nodes incorrect.\n")

          | assertTrue((parserInput->nodes6_len == 0), "nodes6 length was not 0\n")
          | assertTrue((parserInput->values_len == 0), "values length was not 0\n")
          | assertTrue((parserInput->values6_len == 0), "values6 length was not 0\n")
          | assertTrue((parserInput->want_return == -1), "want was not -1\n")
          | assertTrue(parserInput->parser_return == REPLY, "query type not reply for find_node");
    }
    return generic_test("d1:rd2:id20:0123456789abcdefghij5:nodes9:def456...e1:t2:aa1:y1:re",
                        toTest, nullModifier, validateReply);
}

int findNode_reply_test_control(void)
{
    return findNode_reply_tester(parse_message);
}

int findNode_reply_test(void)
{
    return findNode_reply_tester(parse_message1);
}

int main(void)
{
    return
        findNode_test_control()
      | findNode_test()
      | findNode_reply_test_control()
      | findNode_reply_test();
}
