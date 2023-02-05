#ifndef __PROTOCOL_TEST_H__
#define __PROTOCOL_TEST_H__ 1

#ifdef __cplusplus
extern "C" {
#endif

#define TESTER_CRC                  0x00000001UL
#define TESTER_ENDIAN               0x00000002UL
#define TESTER_CONVERT              0x00000004UL
#define TESTER_RAW_P_DMA            0x00000008UL
#define TESTER_REED_SOLOMON_ECC     0x00000010UL
#define TESTER_CALLBACK_MANAGER     0x00000020UL
#define TESTER_PULL_CONTAINER       0x00000040UL
#define TESTER_RAW_P_IT             0x00000080UL
#define TESTER_RAW_P_GEN             0x00000100UL


int protocolAllTest(int randomSeed, int testCnt, unsigned int testFlags);

#ifdef __cplusplus
}
#endif



#endif // __PROTOCOL_TEST_H__
