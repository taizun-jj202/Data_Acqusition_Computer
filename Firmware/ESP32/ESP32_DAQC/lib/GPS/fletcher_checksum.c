/**
 * @file fletcher_checksum.c
 * @brief Calculate Fletcher's 8-bit checksum
 * 
 * This file is used for the calculation of Fletcher's 8-bit checksum
 * in the last two bytes of the UBX message in @file gps.H
 * 
 * @author Taizun J
 * @version 1.0
 * @date 05-01-2025
 */

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

uint8_t disableGGA[] = {
                        0xB5, 0x62, // UBX Header
                        0x06, 0x01, // CFG-MSG 
                        0x03, 0x00, // msg length (LE)
                        0xF0,       // NMEA class 
                        0x00,       // NMEA ID
                        0x00,       // rate set to 0 i.e no transmission
                        0x00, 0x00  // CK_A, CK_B 
};

uint8_t disableGLL[] = {
                        0xB5, 0x62, // UBX Header
                        0x06, 0x01, // CFG-MSG 
                        0x03, 0x00, // msg length (LE)
                        0xF0,       // NMEA class 
                        0x01,       // NMEA ID
                        0x00,       // rate set to 0 i.e no transmission
                        0x00, 0x00  // CK_A, CK_B 
};

uint8_t disableGSA[] = {
                        0xB5, 0x62, // UBX Header
                        0x06, 0x01, // CFG-MSG 
                        0x03, 0x00, // msg length (LE)
                        0xF0,       // NMEA class 
                        0x02,       // NMEA ID
                        0x00,       // rate set to 0 i.e no transmission
                        0x00, 0x00  // CK_A, CK_B 
};

uint8_t disableGSV[] = {
                        0xB5, 0x62, // UBX Header
                        0x06, 0x01, // CFG-MSG 
                        0x03, 0x00, // msg length (LE)
                        0xF0,       // NMEA class 
                        0x03,       // NMEA ID
                        0x00,       // rate set to 0 i.e no transmission
                        0x00, 0x00  // CK_A, CK_B 
};

uint8_t disableRMC[] = {
                        0xB5, 0x62, // UBX Header
                        0x06, 0x01, // CFG-MSG 
                        0x03, 0x00, // msg length (LE)
                        0xF0,       // NMEA class 
                        0x04,       // NMEA ID
                        0x00,       // rate set to 0 i.e no transmission
                        0x00, 0x00  // CK_A, CK_B 
};

uint8_t disableVTG[] = {
                        0xB5, 0x62, // UBX Header
                        0x06, 0x01, // CFG-MSG 
                        0x03, 0x00, // msg length (LE)
                        0xF0,       // NMEA class 
                        0x05,       // NMEA ID
                        0x00,       // rate set to 0 i.e no transmission
                        0x00, 0x00  // CK_A, CK_B 
};`

uint16_t fletcher8_checksum(const uint8_t *data, size_t len) {
    uint8_t sum1 = 0;
    uint8_t sum2 = 0;

    for (size_t i = 2; i < (len-1); i++) {
        
        sum1 = (sum1 + data[i]) % 255;
        sum2 = (sum2 + sum1) % 255;
    }

    return (sum2 << 8) | sum1; // Combine high and low byte into 16-bit checksum
}

int main() {
    // Exclude the last two bytes (CK_A, CK_B) from checksum calculation
    size_t length = sizeof(disableGGA) - 2;

    // Calculate the Fletcher's 8-bit checksum
    uint16_t checksum = fletcher8_checksum(disableGGA, length);
    uint16_t checksum2 = fletcher8_checksum(disableGLL, length);
    uint16_t checksum3 = fletcher8_checksum(disableGSA, length);
    uint16_t checksum4 = fletcher8_checksum(disableGSV, length);
    uint16_t checksum5 = fletcher8_checksum(disableRMC, length);
    uint16_t checksum6 = fletcher8_checksum(disableVTG, length);

    // Print the checksum
    // printf("Length of the data for checksum: %zu bytes\n", length);
    
    // printf("Bytes in disableGGA:\n");
    // for (size_t i = 0; i < sizeof(disableGGA); i++) {
    //     printf("0x%02X ", disableGGA[i]);
    // }
    // printf("\n");

    printf("Fletcher's 8-bit checksum GGA: 0x%04X\n", checksum);
    printf("Fletcher's 8-bit checksum GLL: 0x%04X\n", checksum2);
    printf("Fletcher's 8-bit checksum GSA: 0x%04X\n", checksum3);
    printf("Fletcher's 8-bit checksum GSV: 0x%04X\n", checksum4);
    printf("Fletcher's 8-bit checksum RMC: 0x%04X\n", checksum5);
    printf("Fletcher's 8-bit checksum VTG: 0x%04X\n", checksum6);

    return 0;
}
