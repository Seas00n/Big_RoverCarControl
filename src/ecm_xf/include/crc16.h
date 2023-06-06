#include <stdint.h>
#include <string.h>
#include <stdio.h>

const uint16_t crc16_tab[256] =
{
0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
};
uint16_t crc16(uint8_t *data,uint16_t len)
{
	uint16_t crc16 = 0xFFFF;
	uint32_t uIndex ;
	while (len --)
	{
		uIndex = (crc16&0xff) ^ ((*data) & 0xff) ;
		data = data + 1;
		crc16 = ((crc16>>8) & 0xff) ^ crc16_tab[uIndex];
	}
	return crc16 ;
}

// You can modify these error codes as you see fit
#define ERR_RECEIVE11 11
#define ERR_RECEIVE12 12
#define ERR_CRC     2
#define ERR_LEN     3

uint16_t pack(uint8_t *buffer, uint8_t *data, uint16_t len){
    if (buffer == NULL || data == NULL) {
        return 1; // error: NULL pointer passed
    }
    
    uint8_t header[] = {0x12, 0x34};
    uint8_t tail[] = {0x43, 0x21};
    uint16_t crc = crc16(data, len);  // Calculating the crc16 of data

    memcpy(buffer, header, sizeof(header));                      // Copy header to buffer
    memcpy(buffer + sizeof(header), &len, sizeof(len));          // Copy length to buffer
    memcpy(buffer + sizeof(header) + sizeof(len), &crc, sizeof(crc)); // Copy crc16 to buffer
    memcpy(buffer + sizeof(header) + sizeof(len) + sizeof(crc), data, len); // Copy data to buffer
    memcpy(buffer + sizeof(header) + sizeof(len) + sizeof(crc) + len, tail, sizeof(tail)); // Copy tail to buffer

    return 0; // No error
}

// uint16_t unpack(uint8_t *buffer, uint8_t *data, uint16_t len){
//     uint8_t header[] = {0x12, 0x34};
//     uint8_t tail[] = {0x43, 0x21};
//     uint16_t crc;

//     // Check the header
//     if (memcmp(buffer, header, 2) != 0) {
//         return ERR_RECEIVE11;
//     }
//     // Check the tail
//     if (memcmp(buffer + 4 + len, tail, 2) != 0) {
//         return ERR_RECEIVE12;
//     }
//     // Check the length
//     uint16_t bufferLen;
//     memcpy(&bufferLen, buffer + 2, 2);
//     if (bufferLen != len) {
//         return ERR_LEN;
//     }
//     // Copy crc from buffer
//     memcpy(&crc, buffer + 4, 2);
//     // Verify crc16
//     if (crc != crc16(data, len)) {
//         return ERR_CRC;
//     }
//     // Copy data from buffer
//     memcpy(data, buffer + 6, len);

//     return 0; // No error
// }

uint16_t unpack(uint8_t* buffer, uint8_t* data, uint16_t len) {
    uint8_t header[] = {0x12, 0x34};
    uint8_t tail[] = {0x43, 0x21};
    uint16_t crc;

    // Check the header
    if (memcmp(buffer, header, 2) != 0) {
        printf("Header mismatch: Expected 0x%.2X 0x%.2X, received 0x%.2X 0x%.2X\n",
               header[0], header[1], buffer[0], buffer[1]);
        return ERR_RECEIVE11;
    }
    // Check the tail
    if (memcmp(buffer + 6 + len, tail, 2) != 0) {
        printf("Tail mismatch: Expected 0x%.2X 0x%.2X, received 0x%.2X 0x%.2X\n",
               tail[0], tail[1], buffer[6 + len], buffer[7 + len]);
        return ERR_RECEIVE12;
    }
    // Check the length
    uint16_t bufferLen;
    memcpy(&bufferLen, buffer + 2, 2);
    if (bufferLen != len) {
        printf("Length mismatch: Expected %u, received %u\n", len, bufferLen);
        return ERR_LEN;
    }
        // Copy data from buffer
    memcpy(data, buffer + 6, len);

    
    // Copy crc from buffer
    memcpy(&crc, buffer + 4, 2);
    // Verify crc16
    if (crc != crc16(data, len)) {
        printf("CRC mismatch: Expected %u, received %u\n", crc, crc16(data, len));
        return ERR_CRC;
    }


    return 0; // No error
}


/******************************************************************************
 *	File	:	PdoDefine.h
 *	Version :	1.52
 *	Date	:	2021/11/23
 *	Author	:	XFORCE
 *
 *
 *	Define slave device PDO data structure
 *
 *	You can configure the PDO structure you wanted,
 *	or you can just use the default PDO structure.
 *	No matter what, the example program need to know.
 *
 *
 * @copyright (C) 2020 NEXTW TECHNOLOGY CO., LTD.. All rights reserved.
 *
 ******************************************************************************/
#ifndef _ECM_USR_PDO_DEFINE_H_
#define _ECM_USR_PDO_DEFINE_H_

#define TEST_DRV_CNT			4
#define TEST_IO_CNT				0
#define COUPLER_CNT				0
#define TEST_SLV_CNT			(TEST_DRV_CNT+COUPLER_CNT+TEST_IO_CNT)

#define TEST_CTL_WORD_OFFSET	0
#define TEST_STA_WORD_OFFSET	0

#define TEST_MODEOP		10//CSP Mode

#if (TEST_MODEOP == 4)
#define TARGET_TYPE		int16_t
#elif (TEST_MODEOP == 10)
#define TARGET_TYPE		int16_t
#else
#define TARGET_TYPE		int32_t
#endif

// typedef struct __attribute__((__packed__)) _axis_rxpdo_st_def_t
// {
// 	uint16_t		u16CtlWord;	//	0x6x40 - Control word
// 	TARGET_TYPE		Target;		//	0x6x7A - Target position
// 								//	0x6xFF - Target velocity
// 								//	0x6x71 - Target torque
// }AXIS_RXPDO_ST_DEF_T;
// typedef struct __attribute__((__packed__)) _axis_txpdo_st_def_t
// {
// 	uint16_t		u16StaWord;	//	0x6x41 - Status word
// 	TARGET_TYPE		Actual;		//	0x6x64 - Actual position
// 								//	0x6x6C - Actual velocity
// 								//	0x6x77 - Actual torque
// }AXIS_TXPDO_ST_DEF_T;  
                //  <Index>#x230C</Index>
                // <Name>Vdc</Name>
                // <Type>DINT</Type>
                // <BitSize>32</BitSize>


typedef struct __attribute__((__packed__)) _axis_rxpdo_st_def_t
{
    uint16_t u16CtlWord;
    uint8_t opMode;
    uint8_t complement;
    int32_t tPos;
    int32_t tVel;
    int16_t tTorque;
}AXIS_RXPDO_ST_DEF_T;
typedef struct __attribute__((__packed__)) _axis_txpdo_st_def_t
{
    uint16_t u16StaWord;
    uint8_t opMode;
    uint8_t complement;
    int32_t Pos;
    int32_t vel;
    int16_t Torque;
    uint32_t Vdc;
}AXIS_TXPDO_ST_DEF_T;

typedef struct __attribute__((__packed__)) _io_rxpdo_st_def_t
{
	uint16_t DO;
	uint16_t DO2;
}IO_RXPDO_ST_DEF_T;

typedef struct __attribute__((__packed__)) _io_txpdo_st_def_t
{
	uint16_t DI;
	uint16_t DI2;
}IO_TXPDO_ST_DEF_T;





typedef struct __attribute__((__packed__)) _rxpdo_st_def_t
{
#if (TEST_IO_CNT > 0)
	IO_RXPDO_ST_DEF_T Io[TEST_IO_CNT];
#endif
	AXIS_RXPDO_ST_DEF_T Axis[TEST_DRV_CNT];
}RXPDO_ST_DEF_T;

typedef struct __attribute__((__packed__)) _txpdo_st_def_t
{
#if (TEST_IO_CNT > 0)
	IO_TXPDO_ST_DEF_T Io[TEST_IO_CNT];
#endif
	AXIS_TXPDO_ST_DEF_T Axis[TEST_DRV_CNT];
}TXPDO_ST_DEF_T;
#endif
