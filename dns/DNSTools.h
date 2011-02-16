/**
 * @param domain the input eg my.cool.domain.address.p2p
 * @param labelIndex the number of the label, counting BACK so label 0 will be 'p2p'.
 * @param buffer some space to work, this need not be bigger than domain.
 * @param bufferLength the size of the passed buffer.
 * @return 0 if all goes well, -1 if there aren't that many labels,
 *           -2 if the needed space exceeds the buffer length.
 */
int DNSTools_getDomainLabel(const char* domain, size_t labelIndex, char* buffer, size_t bufferLength);
