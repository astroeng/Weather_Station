/* Derek Schacht
 *  2016 01 07
 *  License : Give me credit where it is due.
 *  Disclaimer : I try and site code that I find on the internet but I am not perfect. If you find
 *               something that should be sited let me know and I will update my code.
 *  Warranty   : Absolutely None
 *
 *  This header also applies to all previous commits. But, I reserve the right to modify this in the future.
 */

#ifndef PHANT_INTERFACE_H
#define PHANT_INTERFACE_H


void createLoggingString(const char* publicKey, const char* privateKey,
                         long* dataArray, const char** stringArray, char numValues);

char* getLogString();

char sendLoggingString(int* device);

#endif
