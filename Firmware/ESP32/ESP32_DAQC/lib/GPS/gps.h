/**
 * @file gps.c
 * @brief GPS Module functionality 
 * 
 * This file contains function definitions and UBX messages.
 * All UBX messages are taken from U-Blox NEO-8M Receiver Description datasheet 
 *  as mentioned in @file DEVNOTES.md . 
 * 
 * All messages defined here will be used in @file gps.c to configure the GPS module. 
 * 
 * Structure of UBX packet is as follows :
 * 
 *  |---------------------------------------------------------------------------------------|
 *  | Syn char 1 | Syn char 2 | CLASS | ID | Length of msg (LE) | ~ payload ~ | CK_A | CK_B |
 *  |---------------------------------------------------------------------------------------|
 * 
 * - Syn char 1 = 0xB5
 * - Syn char 2 = 0x62
 * - CLASS 
 * - ID 
 * - Length of message (Little Endian)
 * - Payload 
 * - Checksum A (Calculated using 8-bit Fletcher Algorithm)
 * - Checksum B
 * 
 * @version 1.0
 * @author Taizun J
 * @date 05-01-2025.
 */

#ifndef _GPS_H_
#define _GPS_H_


#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <HardwareSerial.h>

// Hardware pins 16 and 17 used to UART.
// RX -> 16
// TX -> 17
HardwareSerial gpsSerial(2);


/**
 * List of functions :
 * 
 * 1. calc_and_append_checksum() :
 * 2. sendUBXCommand() :
 * 3. configureGPS () :
 *   - set baud rate to 9600 
 *   - disable NMEA msgs via UBX commands 
 *   - enable NAV-POSLLH via UBX command 
 * 
 */


//--------------------------------------------------------------------------------------------
// NMEA messages to be disabled
// Following messages are disabled :
//   1. GGA
//   2. GLL
//   3. GSA
//   4. GSV
//   5. RMC
//   6. VTG

// For the following messages below, checksum has been calculated as follows :
    // Fletcher's 8-bit checksum GGA: 0x17FA
    // Fletcher's 8-bit checksum GLL: 0x18FB
    // Fletcher's 8-bit checksum GSA: 0x19FC
    // Fletcher's 8-bit checksum GSV: 0x1AFD
    // Fletcher's 8-bit checksum RMC: 0x1BFE
    // Fletcher's 8-bit checksum VTG: 0x1C00
//--------------------------------------------------------------------------------------------

uint8_t disableGGA[] = {
                        0xB5, 0x62, // UBX Header
                        0x06, 0x01, // CFG-MSG 
                        0x03, 0x00, // msg length (LE)
                        0xF0,       // NMEA class 
                        0x00,       // NMEA ID
                        0x00,       // rate set to 0 i.e no transmission
                        0X17, 0xFA  // CK_A, CK_B 
};

uint8_t disableGLL[] = {
                        0xB5, 0x62, // UBX Header
                        0x06, 0x01, // CFG-MSG 
                        0x03, 0x00, // msg length (LE)
                        0xF0,       // NMEA class 
                        0x01,       // NMEA ID
                        0x00,       // rate set to 0 i.e no transmission
                        0x18, 0xFB  // CK_A, CK_B 
};

uint8_t disableGSA[] = {
                        0xB5, 0x62, // UBX Header
                        0x06, 0x01, // CFG-MSG 
                        0x03, 0x00, // msg length (LE)
                        0xF0,       // NMEA class 
                        0x02,       // NMEA ID
                        0x00,       // rate set to 0 i.e no transmission
                        0x19, 0xFC  // CK_A, CK_B 
};

uint8_t disableGSV[] = {
                        0xB5, 0x62, // UBX Header
                        0x06, 0x01, // CFG-MSG 
                        0x03, 0x00, // msg length (LE)
                        0xF0,       // NMEA class 
                        0x03,       // NMEA ID
                        0x00,       // rate set to 0 i.e no transmission
                        0x1A, 0xFD  // CK_A, CK_B 
};

uint8_t disableRMC[] = {
                        0xB5, 0x62, // UBX Header
                        0x06, 0x01, // CFG-MSG 
                        0x03, 0x00, // msg length (LE)
                        0xF0,       // NMEA class 
                        0x04,       // NMEA ID
                        0x00,       // rate set to 0 i.e no transmission
                        0x1B, 0xFE  // CK_A, CK_B 
};

uint8_t disableVTG[] = {
                        0xB5, 0x62, // UBX Header
                        0x06, 0x01, // CFG-MSG 
                        0x03, 0x00, // msg length (LE)
                        0xF0,       // NMEA class 
                        0x05,       // NMEA ID
                        0x00,       // rate set to 0 i.e no transmission
                        0x1C, 0x00  // CK_A, CK_B 
};


/**
 * -------------------------------------------------------------------------------------------
 * Commands to enable NAV-POSLLH message : 
 * -------------------------------------------------------------------------------------------
 */

uint8_t poll_NAV_POSLLH[] = {
                             0xB5, 0x62, // UBX Header
                             0x01, 0x02, // NAV-POSLLH
                             0x00, 0x00, // msg length (LE)
                             0x0A, 0x06  // CK_A, CK_B
};


//--------------------------------------------------------------------------------------------
// NAV-POSLLH message structure
//  To be used to store data recieved from NAV-POSLLH message
//--------------------------------------------------------------------------------------------

typedef struct {
    uint32_t iTOW;      // time of week in milliseconds
    int32_t lon;        // longitude in degrees
    int32_t lat;        // latitude in degrees
    int32_t height;     // height above ellipsoid in mm
    int32_t hMSL;       // height above mean sea level in mm
    uint32_t hAcc;      // horizontal accuracy in mm
    uint32_t vAcc;      // vertical accuracy in mm
} NAV_POSLLH_t;



//--------------------------------------------------------------------------------------------
// ERROR messages for GPS module
///--------------------------------------------------------------------------------------------
typedef enum {
    GPS_OK = 0,         // No error
    GPS_HEADER_OK,      // Header OK
    GPS_PAYLOAD_OK,     // Payload OK
    GPS_SEND_CMD_OK,

    GPS_HEADER_ERROR, // Invalid header error
    GPS_PAYLOAD_ERROR,  // Payload error
    GPS_SEND_CMD_ERROR,     // Error in sending command
} GPS_Error_t;



//--------------------------------------------------------------------------------------------
// Function prototypes
//--------------------------------------------------------------------------------------------

/**
 * @brief Initializes GPS module by disabling NMEA and enabling UBX messages
 * @param gpsSerial : SoftwareSerial object for GPS module
 * @return None
 */
void configureGPS(HardwareSerial* gpsSerial);


/**
 * @brief Sends UBX command to GPS module
 * @param[in] gpsSerial : SoftwareSerial object for GPS module
 * @param[in] command UBX Command to be sent to GPS module
 * @param[in] len Length of UBX command
 * @return None
 */
void sendUBXCommand(HardwareSerial* gpsSerial, uint8_t* command, size_t len);


/**
 * @brief Get UBX message from GPS module
 * 
 * Reads UBX message recieved from GPS module after sending 
 * NAV-POSLLH message to GPS module. Reads value using 
 * serial object from `HardwareSerial` class
 * 
 * Message variable entered here must have a length of 28 bytes
 * 
 * @param[in] gpsSerial : SoftwareSerial object for GPS module
 * @param[out] message : Pointer to array to store message
 */
void readUBXMessage(HardwareSerial* gpsSerial, uint8_t* message);


#endif /* _GPS_H_ */


