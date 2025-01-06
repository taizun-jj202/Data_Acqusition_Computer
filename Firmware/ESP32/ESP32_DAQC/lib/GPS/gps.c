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


#include <stdint.h>
#include "gps.h"

/**
 * List of functions :
 * 
 * 1. calc_and_append_checksum()
 * 2. sendUBXCommand() 
 * 3. configureGPS () :
 *   - set baud rate to 9600 
 *   - disable NMEA msgs via UBX commands 
 *   - enable NAV-POSLLH via UBX command 
 * 4. readUBXMessage() 
 */



/**
 * @brief Initializes GPS module by disabling NMEA and enabling UBX messages
 * @param[in] gpsSerial SoftwareSerial object for GPS module    
 * @return None
 */
void configureGPS(HardwareSerial* gpsSerial) {

    
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
        gpsSerial->write(command[i]);       // Send byte to GPS module 
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

    if (gpsSerial->read() != 0xB5 || gpsSerial->read() != 0x62) {
        printf("Error: Invalid UBX message header\n");
        return GPS_HEADER_ERROR;
    }


    // Read 28B of payload into payload array
    for(uint8_t i = 0; i < 28; i++) {
        message[i] = gpsSerial->read();
    } 

    // return GPS_PAYLOAD_OK
}