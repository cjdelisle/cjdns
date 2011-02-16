#define DNS_RETURN_NO_ERROR        0  /* No error. The request completed successfully.	RFC 1035 */
#define DNS_RETURN_FORMAT_ERROR    1  /* The name server was unable to interpret the query.	RFC 1035 */
#define DNS_RETURN_SERVER_FAILURE  2  /* The name server was unable to process this query due to a problem
                                       * with the name server.	RFC 1035
                                       */
#define DNS_RETURN_NAME_ERROR      3	 /* Meaningful only for responses from an authoritative name server,
                                       * this code signifies that the domain name referenced in the query
                                       * does not exist.	RFC 1035
                                       */
#define DNS_RETURN_NOT_IMPLEMENTED 4	 /* The name server does not support the requested kind of query.
                                       * RFC 1035
                                       */
#define DNS_RETURN_REFUSED         5	 /* The name server refuses to perform the specified operation
                                       * for policy reasons. For example, a name server may not wish to 
                                       * provide the information to the particular requester, or a name
                                       * server may not wish to perform a particular operation
                                       * (e.g., zone transfer) for particular data. RFC 1035
                                       */
#define DNS_RETURN_YXDOMAIN        6	 /* Name Exists when it should not.	RFC 2136 */
#define DNS_RETURN_YXRRSET         7	 /* RR Set Exists when it should not.	RFC 2136 */
#define DNS_RETURN_NXRRSET         8	 /* RR Set that should exist does not.	RFC 2136 */
#define DNS_RETURN_NOT_AUTH        9	 /* Server Not Authoritative for zone.	RFC 2136 */
#define DNS_RETURN_NOT_IN_ZONE     10 /* Name not contained in zone. */

#define DNS_RETURN_BASVERS         16	/* Bad OPT Version. RFC 2671 */
#define DNS_RETURN_BADSIG          16 /* TSIG Signature Failure.	RFC 2845 */

#define DNS_RETURN_BADKEY          17	/* Key not recognized.	RFC 2845 */
#define DNS_RETURN_BADTIME         18	/* Signature out of time window.	RFC 2845 */
#define DNS_RETURN_BADMODE         19	/* Bad TKEY Mode.	RFC 2930 */
#define DNS_RETURN_BADNAME         20	/* Duplicate key name.	RFC 2930 */
#define DNS_RETURN_BADALG          21	/* Algorithm not supported.	RFC 2930 */
#define DNS_RETURN_BADTRUNC        22	/* Bad truncation. */
