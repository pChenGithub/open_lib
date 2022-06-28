#ifndef _CRC16_XMODEM_H_
#define _CRC16_XMODEM_H_

#ifdef __cplusplus
extern "C" {
#endif

#define CRC16_CHECK_SUCCESS 0
#define CRC16_CHECK_FAILED  1
unsigned short crc16_xmodem(unsigned char *ptr, unsigned int len);
int crc16_check(unsigned char* buffer, const int len, unsigned short crc16);

int crc16_fileCheck(const char* file, const unsigned short crc16);

#ifdef __cplusplus
}
#endif


#endif


