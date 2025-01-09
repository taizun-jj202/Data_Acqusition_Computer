/**
 * @file gps.c
 * @brief GPS Module functionality 
 * 
 * This file contains all required functions and definitions 
 * to interface with Ublox NEO-8M GPS Module. 
 * Since the GPS module will be used to track and poll time 
 * and other data given below at a higher than usual rate because of 
 * high speed of the vehicle, the module will be interfaced using 
 * proprietary UBX protocol from U-Blox over hardware UART interface. 
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
 * 
 * 
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
#include <Arduino.h>
#include <HardwareSerial.h>

#define GPS_RX 16
#define GPS_TX 17
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
// Function prototypes
//--------------------------------------------------------------------------------------------



/**
 * @brief Initializes GPS module by disabling NMEA and enabling UBX messages
 * @param[in] gpsSerial SoftwareSerial object for GPS module    
 * @return None
 */
void configureGPS(HardwareSerial* gpsSerial) {

    gpsSerial.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX ); // Set baud rate to 9600


    
    // Disable NMEA messages
    sendUBXCommand(gpsSerial, disableGGA, sizeof(disableGGA));
    sendUBXCommand(gpsSerial, disableGLL, sizeof(disableGLL));
    sendUBXCommand(gpsSerial, disableGSA, sizeof(disableGSA));
    sendUBXCommand(gpsSerial, disableGSV, sizeof(disableGSV));
    sendUBXCommand(gpsSerial, disableRMC, sizeof(disableRMC));
    sendUBXCommand(gpsSerial, disableVTG, sizeof(disableVTG));
    
    // Enable NAV-POSLLH message
    sendUBXCommand(gpsSerial, poll_NAV_POSLLH, sizeof(poll_NAV_POSLLH));
}


/**
 * @brief Sends UBX command to GPS module
 * @param[in] gpsSerial SoftwareSerial object for GPS module
 * @param[in] command Command to be sent to GPS module
 * @param[in] length Length of command
 * @return None
 */
void sendUBXCommand(HardwareSerial_h* gpsSerial, uint8_t *command, uint8_t length) {
    
    // Send command to GPS module
    for (uint8_t i = 0; i < length; i++) {
        gpsSerial.write(command[i]);       // Send byte to GPS module 
    }
    gpsSerial->flush();

    // return GPS_SEND_CMD_OK;
};



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
void readUBXMessage(HardwareSerial* gpsSerial, uint8_t* message) {
    

    /** 
     * 
     * Message variable entered here must have a length of 28 bytes
     * 
     * Contains only the following : 
     * - U4 bytes : iTOW (time of week in milliseconds)
     * - I4 bytes : lon (longitude in degrees)
     * - I4 bytes : lat (latitude in degrees)
     * - I4 bytes : height (height above ellipsoid in mm)
     * - I4 bytes : hMSL (height above mean sea level in mm)
     * - U4 bytes : hAcc (horizontal accuracy in mm)
     * - U4 bytes : vAcc (vertical accuracy in mm)
     */

    // Send NAV-POSLLH message to GPS module
    sendUBXCommand(gpsSerial, poll_NAV_POSLLH, sizeof(poll_NAV_POSLLH));

    if (gpsSerial.read() != 0xB5 || gpsSerial.read() != 0x62) {
        printf("Error: Invalid UBX message header\n");
        // return GPS_HEADER_ERROR;
    }


    // Read 28B of payload into payload array
    for(uint8_t i = 0; i < 28; i++) {
        message[i] = gpsSerial.read();
    } 

    // return GPS_PAYLOAD_OK
}


#endif // _GPS_H_