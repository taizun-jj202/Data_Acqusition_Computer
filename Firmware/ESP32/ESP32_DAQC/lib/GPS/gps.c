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