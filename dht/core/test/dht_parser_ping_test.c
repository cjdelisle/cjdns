#include "dht_parser_test_framework.c"

static int ping_tester(int(*toTest)(const unsigned char *buf, int buflen,
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
    int validatePing(struct dht_parser_input *parserInput)
    {
        char twenty_os[20];
        memset(twenty_os, 0, 20);

        return
            assertTrue((parserInput->tid_len == 2), "transactionId is wrong length\n")
          | assertTrue(0 == memcmp(parserInput->tid_return, "aa", 1),
                       "transactionId is not 'aa'\n")
          | assertTrue(0 == memcmp(parserInput->id_return, "abcdefghij0123456789", 20),
                      "peer's id is incorrect\n")
          | assertTrue(0 == memcmp(parserInput->info_hash_return, twenty_os, 20),
                       "infoHash was set for a ping!\n")
          | assertTrue(0 == memcmp(parserInput->target_return, twenty_os, 20),
                       "target was set for a ping!\n")
          | assertTrue((parserInput->port_return == 0), "port was not set to 0\n")
          | assertTrue((parserInput->token_len == 0), "token length was not 0\n")
          | assertTrue((parserInput->nodes_len == 0), "nodes length was not 0\n")
          | assertTrue((parserInput->nodes6_len == 0), "nodes6 length was not 0\n")
          | assertTrue((parserInput->values_len == 0), "values length was not 0\n")
          | assertTrue((parserInput->values6_len == 0), "values6 length was not 0\n")
          | assertTrue((parserInput->want_return == -1), "want was not -1\n")
          | assertTrue(parserInput->parser_return == PING, "query type not ping");
    }
    return generic_test("d1:ad2:id20:abcdefghij0123456789e1:q4:ping1:t2:aa1:y1:qe",
                        toTest, nullModifier, validatePing);
}

int ping_test_control(void)
{
    return ping_tester(parse_message);
}

int ping_test(void)
{
    return ping_tester(parse_message1);
}

static int pong_tester(int(*toTest)(const unsigned char *buf, int buflen,
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
    int validatePong(struct dht_parser_input *parserInput)
    {
        char twenty_os[20];
        memset(twenty_os, 0, 20);

        return
            assertTrue((parserInput->tid_len == 2), "transactionId is wrong length\n")
          | assertTrue(0 == memcmp(parserInput->tid_return, "aa", 1),
                       "transactionId is not 'aa'\n")
          | assertTrue(0 == memcmp(parserInput->id_return, "mnopqrstuvwxyz123456e1", 20),
                      "peer's id is incorrect\n")
          | assertTrue(0 == memcmp(parserInput->info_hash_return, twenty_os, 20),
                       "infoHash was set for a pong!\n")
          | assertTrue(0 == memcmp(parserInput->target_return, twenty_os, 20),
                       "target was set for a pong!\n")
          | assertTrue((parserInput->port_return == 0), "port was not set to 0\n")
          | assertTrue((parserInput->token_len == 0), "token length was not 0\n")
          | assertTrue((parserInput->nodes_len == 0), "nodes length was not 0\n")
          | assertTrue((parserInput->nodes6_len == 0), "nodes6 length was not 0\n")
          | assertTrue((parserInput->values_len == 0), "values length was not 0\n")
          | assertTrue((parserInput->values6_len == 0), "values6 length was not 0\n")
          | assertTrue((parserInput->want_return == -1), "want was not -1\n")
          | assertTrue(parserInput->parser_return == REPLY, "message type not reply for pong");
    }
    return generic_test("d1:rd2:id20:mnopqrstuvwxyz123456e1:t2:aa1:y1:re",
                        toTest, nullModifier, validatePong);
}

int pong_test_control(void)
{
    return pong_tester(parse_message);
}

int pong_test(void)
{
    return pong_tester(parse_message1);
}

int main(void)
{
    return
        ping_test_control()
      | ping_test()
      | pong_test_control()
      | pong_test();
}
