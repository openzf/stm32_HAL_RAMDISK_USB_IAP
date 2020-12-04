#ifndef __KYMODEM_H
#define __KYMODEM_H

#include "common.h"


#define SOH (0x01)      /* start of 128-byte data packet */
#define STX (0x02)      /* start of 1024-byte data packet */
#define EOT (0x04)      /* end of transmission */
#define ACK (0x06)      /* receive OK */
#define NAK (0x15)      /* receiver error; retry */
#define CA (0x18)      /* two of these in succession aborts transfer */
#define CNC (0x43)      /* character 'C' */


#define FILE_NAME_LENGTH        ((uint32_t)64)
#define FILE_SIZE_LENGTH        ((uint32_t)16)

#define PACKET_HEADER_SIZE      ((uint32_t)3)

#define PACKET_DATA_INDEX       ((uint32_t)3)
#define PACKET_START_INDEX      ((uint32_t)0)
#define PACKET_NUMBER_INDEX     ((uint32_t)1)
#define PACKET_CNUMBER_INDEX    ((uint32_t)2)



#define PACKET_TRAILER_SIZE     ((uint32_t)2)
#define PACKET_OVERHEAD_SIZE    (PACKET_HEADER_SIZE + PACKET_TRAILER_SIZE - 1)
#define PACKET_SIZE             ((uint32_t)128)
#define PACKET_1K_SIZE          ((uint32_t)1024)

#define PACKET_SENT_MODE_128BYTE   0
#define PACKET_SENT_MODE_1024BYTE  1
#define PACKET_SENT_MODE_AUTO      2

#define SOH_PACKET_SIZE  128
#define STX_PACKET_SIZE  1024

// 协议帧
typedef struct {
    uint8_t head;
    uint8_t index;
    uint8_t re_index;
    uint8_t data[1024];
    uint8_t crc_H;
    uint8_t crc_L;
} ModemFrameTypedef;


// 协议帧状态
typedef struct {
    uint8_t frame_state;
    uint32_t frame_data_len;
    uint32_t frame_data_now_index;
    uint16_t frame_crc;
} ModemFrameStateTypedef;

// 协议解析器状态
typedef enum {
    FRAME_FIND_HEAD =0,
    FRAME_FIND_INDEX,
    FRAME_FIND_REINDEX,
    FRAME_FIND_DATA,
    FRAME_FIND_CRC
} FrameRXState;

// 解析器返回状态
#define FRAME_PARSER_RUN  0
#define FRAME_PARSER_OK  1
#define FRAME_PARSER_HEAE_ERROR  -1
#define FRAME_PARSER_REINDEX_NOTRQ_ERROR  -2
#define FRAME_PARSER_CRC_ERROR  -3

#define FRAME_PARSER_HEAD_NOT_HAND  -4
#define FRAME_PARSER_HEAD_NOT_FIND  -5
#define FRAME_PARSER_DATA_REC_ERROR  -6
#define FRAME_PARSER_END_REC_ERROR  -7
#define FRAME_PARSER_USER_HAND_HEAD_ERROR  -8
#define FRAME_PARSER_ABORT_ERROR  -9
#define FRAME_PARSER_DATA_SIZE_NOTEQ_ERROR  -10


#define PACKET_RX_FINISH_OK  0
#define PACKET_RX_ABORT_ERROR  -1
#define PACKET_RX_DATA_SIZE_NOTEQ_ERROR  -2

#define FILE_NAME_SIZE 255
// 传输的文件
typedef struct
{
    uint8_t name_data[FILE_NAME_SIZE];
    uint32_t name_data_size;
    uint32_t file_size;
    uint32_t sent_rec_file_size;
		int remain_file_size;
		uint8_t  percent;
		uint8_t  *file_ptr;
    uint8_t  is_rec_ok;
} PacketFile;


// packet
typedef struct {
    PacketFile packet_file;
    char packet_state;
		uint8_t last_packet_index;
		uint8_t now_packet_index;
} ModemPacketTypedef;

// packet rx状态
typedef enum {
    PACKET_RX_WAIT =0,
    PACKET_RX_FIND_HEAD,
    PACKET_RX_FIND_DATA,
    PACKET_RX_FIND_END
} PacketRXState;

// packet状态
typedef enum {
    PACKET_TX_WAIT =0,
    PACKET_TX_WAIT_HEAD_ACK,
    PACKET_TX_WAIT_SENT_DATA_ACK,
    PACKET_TX_WAIT_END_DATA_ACK,
    PACKET_TX_WAIT_FIRST_EOT_ACK_NCK,
    PACKET_TX_WAIT_EOT_WAIT_ACK,
    PACKET_TX_WAIT_END_C,
} PacketTXState;



typedef struct Ymodem_TypeDef_TAG
{
	void (*ymodem_rx_error_handle)(int error_code);
	char (*ymodem_rx_head_handle)(char *file_name,uint16_t file_name_len, uint32_t file_len);
	void (*ymodem_rx_data_handle)(char *data, uint16_t len,uint32_t download_byte,uint8_t percent);
	void (*ymodem_rx_finish_handle)(int data);
	void (*ymodem_write_byte)(int state);
	void (*ymodem_tx_data_handle)(uint8_t **file_read_addr, uint32_t  file_read_size, uint32_t file_has_read_size,  uint32_t file_remain_size,uint8_t percent);
}Ymodem_TypeDef;

// 初始化
void ymodem_init(Ymodem_TypeDef *ymodem);


void ymodem_rx_handle(uint8_t *data,uint32_t len);
void ymodem_rx_time_handle(void);


void ymodem_tx_init(char *file_name,char file_name_len,uint32_t file_size);
void ymodem_tx_handle(uint8_t  *buf, uint32_t sz);
void ymodem_tx_time_handle(void);
#endif
