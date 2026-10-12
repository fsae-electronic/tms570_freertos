/**
 * @file xbee.h
 * @brief XBee API frame builder and parser
 * @note Frames: - AT Command 0x08
 *               - Transmit Request 0x10
 *               - AT Response 0x88
 *               - TX Status 0x89
 *               - RX Packet 0x90
 */

#ifndef _XBEE_H_
#define _XBEE_H_

// Headers /////////////////////////////////////////////////////////////////////////////

#include <stddef.h>
#include <stdint.h>

// Defines /////////////////////////////////////////////////////////////////////////////

#define XBEE_API_FRAME_HEAD_SZ (4U) // StartDelimiter | Length MSB | Length LSB | FrameType

#define XBEE_API_DATA_HEAD_SZ_AT_COMMAND (3U)  // FrameID | 16-bit ATCmd
#define XBEE_API_DATA_HEAD_SZ_TX_REQUEST (15U) // FrameID | 64-bit Dest | 16-bit Dest | BroadcastRadius | TxOptions
#define XBEE_API_DATA_HEAD_SZ_AT_RESPONSE (4U) // FrameID | 16-bit ATCmd | CmdStatus
#define XBEE_API_DATA_HEAD_SZ_TX_STATUS (2U)   // FrameID | DeliveryStatus
#define XBEE_API_DATA_HEAD_SZ_RX_PACKET (13U)  // 64-bit Src | 16-bit Src | RxOptions

/* ---------------------------------------------+--------------+
 * Radio                                        | Payload Size |
 * ---------------------------------------------+--------------+
 * 802.15.4 / XBee                              | 94 Bytes     |
 * XTend / XTC (Xtend Compatible) / SX products | 2048 Bytes   |
 * 900 HP                                       | 256 Bytes    |
 * ---------------------------------------------+--------------+
 */
#define XBEE_API_MAX_PAYLOAD_SZ (0xFFU) // Set max payload based on device and application requirements

#define XBEE_API_CHECKSUM_SZ (1U)

/* FrameHeader | (Max)DataHeader | Payload | Checksum */
#define XBEE_API_MAX_DATA_SZ (XBEE_API_DATA_HEAD_SZ_TX_REQUEST + XBEE_API_MAX_PAYLOAD_SZ)
#define XBEE_API_MAX_FRAME_SZ (XBEE_API_FRAME_HEAD_SZ + XBEE_API_MAX_DATA_SZ + XBEE_API_CHECKSUM_SZ)

// #define XBEE_API_MAX_PENDING_FRAMES 10

#define XBEE_API_FUNC_PROTO(func, frame) XBeeAPIStatus_t func(XBeeAPIFrame_t *frame)

// Types ///////////////////////////////////////////////////////////////////////////////

typedef enum
{
    XBEE_API_TYPE_AT_COMMAND = 0x08,
    XBEE_API_TYPE_TX_REQUEST = 0x10,
    XBEE_API_TYPE_AT_RESPONSE = 0x88,
    XBEE_API_TYPE_TX_STATUS = 0x89,
    XBEE_API_TYPE_RX_PACKET = 0x90
} XBeeAPIFrameType_t;

typedef enum
{
    XBEE_API_INIT_SUCCESS,
    XBEE_API_INIT_ERROR,
    // XBEE_API_RX_WAITING_START,
    // XBEE_API_RX_READING,
    XBEE_API_RX_SUCCESS,
    XBEE_API_RX_ERROR_NO_DATA,
    XBEE_API_RX_ERROR_FRAME_TOO_LARGE,
    XBEE_API_RX_ERROR_INVALID_CHECKSUM,
    XBEE_API_RX_ERROR_INVALID_FRAME,
    XBEE_API_TX_SUCCESS,
    XBEE_API_TX_ERROR_FRAME_TOO_LARGE,
    XBEE_API_TX_ERROR_INVALID_FRAME
} XBeeAPIStatus_t;

typedef enum
{
    /* Special commands */
    XBEE_API_AT_CMD_AC = 0x4143, // Apply Changes
    XBEE_API_AT_CMD_FR = 0x4652, // Software Reset
    XBEE_API_AT_CMD_WR = 0x5752, // Write

    /* MAC/PHY commands */
    XBEE_API_AT_CMD_HP = 0x4850, // Preamble ID
    XBEE_API_AT_CMD_IP = 0x4950, // Network ID
    XBEE_API_AT_CMD_MT = 0x4D54, // Broadcast Multi-Transmits
    XBEE_API_AT_CMD_BR = 0x4252, // RF Data Rate
    XBEE_API_AT_CMD_PL = 0x504C, // TX Power Level
    XBEE_API_AT_CMD_RR = 0x5252, // Unicast Mac Retries

    /* Diagnostic commands - MAC statistics and timeouts */
    XBEE_API_AT_CMD_BC = 0x4243, // Bytes Transmitted
    XBEE_API_AT_CMD_DB = 0x4442, // Last Packet RSSI

    /* Network commands */
    XBEE_API_AT_CMD_CE = 0x4345, // Routing / Messaging Mode

    /* Addressing commands */
    XBEE_API_AT_CMD_SH = 0x5348, // Serial Number High
    XBEE_API_AT_CMD_SL = 0x534C, // Serial Number Low
    XBEE_API_AT_CMD_DH = 0x4448, // Destination Address High
    XBEE_API_AT_CMD_DL = 0x444C, // Destination Address Low
    XBEE_API_AT_CMD_TO = 0x544F, // Transmit Options
    XBEE_API_AT_CMD_NI = 0x4E49, // Node Identifier
    XBEE_API_AT_CMD_NT = 0x4E54, // Network Discovery Back-off
    XBEE_API_AT_CMD_NO = 0x4E4F, // Network Discovery Options
    XBEE_API_AT_CMD_CI = 0x4349, // Cluster ID

    /* Addressing discovery/configuration commands */
    XBEE_API_AT_CMD_DN = 0x444E, // Discover Node
    XBEE_API_AT_CMD_ND = 0x4E44, // Network Discover
    XBEE_API_AT_CMD_FN = 0x464E, // Find Neighbors

    /* Security commands */
    XBEE_API_AT_CMD_EE = 0x4545, // Encryption Enable
    XBEE_API_AT_CMD_KY = 0x4B59, // AES Encryption Key

    /* Serial interfacing commands */
    XBEE_API_AT_CMD_BD = 0x4244, // Interface Data Rate
    XBEE_API_AT_CMD_NB = 0x4E42, // Parity
    XBEE_API_AT_CMD_SB = 0x5342, // Stop Bits
    XBEE_API_AT_CMD_AP = 0x4150, // API Enable
    XBEE_API_AT_CMD_AO = 0x414F, // API Options

    /* Sleep commands */
    XBEE_API_AT_CMD_SM = 0x534D, // Sleep Mode
    XBEE_API_AT_CMD_SO = 0x534F, // Sleep Options

    /* Command mode options */
    XBEE_API_AT_CMD_CC = 0x4343, // Command Sequence Character
    XBEE_API_AT_CMD_CT = 0x4354, // Command Mode Timeout
    XBEE_API_AT_CMD_CN = 0x434E, // Exit Command Mode

    /* Firmware version/information commands */
    XBEE_API_AT_CMD_NP = 0x4E50, // Maximum Packet Payload Bytes

    XBEE_API_AT_CMD_UNKNOWN = 0xFFFF // Unknown command
} XBeeAPIATCommand_t;

typedef enum
{
    XBEE_API_CMD_STATUS_OK = 0x00,
    XBEE_API_CMD_STATUS_ERROR = 0x01,
    XBEE_API_CMD_STATUS_INVALID_CMD = 0x02,
    XBEE_API_CMD_STATUS_INVALID_PARAM = 0x03
} XBeeAPICommandStatus_t;

typedef enum
{
    XBEE_API_TX_STATUS_SUCCESS = 0x00,
    XBEE_API_TX_STATUS_INVALID_FRAME = 0x2C,
    XBEE_API_TX_STATUS_INTERNAL_FAILURE = 0x31,
    XBEE_API_TX_STATUS_RESOURCE_ERROR = 0x32,
    XBEE_API_TX_STATUS_PAYLOAD_TOO_LARGE = 0x74,
    XBEE_API_TX_STATUS_INVALID_HOST_ADDRESS = 0x7A,
    XBEE_API_TX_STATUS_INVALID_DATA_MODE = 0x7B,
    XBEE_API_TX_STATUS_INVALID_INTERFACE = 0x7C,
    XBEE_API_TX_STATUS_CONECTION_REFUSED = 0x80
} XBeeAPIDeliveryStatus_t;

// typedef struct
// {
//     // uint32_t networkID; // Network ID (PAN ID)
//     // uint8_t rfDataRate; // RF data rate in kbps
//     // uint8_t retries;    // Unicast MAC retries
//     // uint8_t txPower;    // Transmission power level

//     uint64_t dest64;    // 64-bit destination address
//     uint16_t dest16;    // 16-bit destination address
//     uint16_t txOptions; // Transmission options (bitmask)

//     // uint8_t encryptionEnabled; // Flag indicating if encryption is enabled
//     // uint8_t encryptionKey[64]; // AES encryption key (256-bit)

//     // uint32_t baudRate; // Baud rate for serial communication
//     // uint8_t parity;    // Parity setting (0: None, 1: Even, 2: Odd)
//     // uint8_t stopBits;  // Number of stop bits (1 or 2)
// } XBeeAPIConfig_t;

typedef struct
{
    uint8_t frameID;
    XBeeAPIATCommand_t ATCmd;
    uint8_t *param; // Optional parameter bytes
    uint8_t paramLen;
} XBeeAPIATCommandFrame_t;

typedef struct
{
    uint8_t frameID;
    uint64_t dest64;
    uint16_t dest16;
    uint8_t broadcastRadius;
    // struct
    // {
    //     uint8_t disableACK : 1;
    //     uint8_t disableRouteDiscovery : 1;
    //     uint8_t unicastNACK : 1;
    //     uint8_t unicastTraceRoute : 1;
    //     uint8_t reserved : 1;
    //     uint8_t deliveryMethod : 2;
    // } txOptions;
    uint8_t txOptions;
    uint8_t *payloadData;
    uint16_t payloadDataLen;
} XBeeAPITxRequestFrame_t;

typedef struct
{
    uint8_t frameID;
    XBeeAPIATCommand_t ATCmd;
    XBeeAPICommandStatus_t cmdStatus;
    uint8_t *cmdData; // Optional command bytes
    uint8_t cmdDataLen;
} XBeeAPIATResponseFrame_t;

typedef struct
{
    uint8_t frameID;
    XBeeAPIDeliveryStatus_t deliveryStatus;
} XBeeAPITxStatusFrame_t;

typedef struct
{
    uint64_t src64;
    uint16_t src16;
    // struct
    // {
    //     uint8_t packetACK : 1;
    //     uint8_t packetBroadcast : 1;
    //     uint8_t reserved : 4;
    //     uint8_t deliveryMethod : 2;
    // } rxOptions;
    uint8_t rxOptions;
    uint8_t *rxData;
    uint16_t rxDataLen;
} XBeeAPIRxPacketFrame_t;

typedef struct
{
    uint16_t length; // Frame len for Tx, data+csum len for Rx
    XBeeAPIFrameType_t type;
    union
    {
        XBeeAPIATCommandFrame_t ATCommand;
        XBeeAPITxRequestFrame_t txRequest;
        XBeeAPIATResponseFrame_t ATResponse;
        XBeeAPITxStatusFrame_t txStatus;
        XBeeAPIRxPacketFrame_t rxPacket;
    } data;
    // uint8_t checksum;

    /* This buffer is used for constructing frames before transmission and for storing
     * received frames before parsing. For Tx, store the payload data at position
     * XBEE_API_FRAME_HEAD_SZ + XBEE_API_DATA_HEAD_SZ_x and update the provided ptr. For
     * Rx, store the received bytes in this buffer and retrieve the parsed frame from
     * the appropriate structure. The ptr will point to the start of the payload.
     */
    uint8_t buffer[XBEE_API_MAX_FRAME_SZ];

    /* NOTE: XBee devices are Big-Endian */
} XBeeAPIFrame_t;

// Config Functions ////////////////////////////////////////////////////////////////////

/**
 * @brief Initializes the XBee API
 * @param[in] config Pointer to the XBee API configuration structure
 * @return XBeeAPIStatus_t The status of the operation
 */
// XBeeAPIStatus_t XBeeAPIInit(XBeeAPIConfig_t *config);

// TxRx Functions //////////////////////////////////////////////////////////////////////

/**
 * @brief Builds/Parses an XBee API frame
 * @param[in] frame Pointer to the XBeeAPIFrame_t structure to be built/parsed
 * @return XBeeAPIStatus_t The status of the build/parsing operation
 */
XBEE_API_FUNC_PROTO(XBeeAPIBuildFrame, frame);
XBEE_API_FUNC_PROTO(XBeeAPIParseFrame, frame);

////////////////////////////////////////////////////////////////////////////////////////

#endif /* _XBEE_H_ */
