#ifndef __PROTOCOL_TEST_H__
#define __PROTOCOL_TEST_H__ 1

#ifdef __cplusplus
extern "C" {
#endif

#define TESTER_CRC         0x00000001UL
#define TESTER_ENDIAN      0x00000002UL
#define TESTER_CONVERT     0x00000004UL
#define TESTER_RAW_P_DMA   0x00000008UL


int protocolAllTest(int randomSeed, int testCnt, unsigned int testFlags);

#ifdef __cplusplus
}
#endif



#endif // __PROTOCOL_TEST_H__
