/* Derek Schacht
 *  2016 01 07
 *  License : Give me credit where it is due. 
 *  Disclaimer : I try and site code that I find on the internet but I am not perfect. If you find 
 *               something that should be sited let me know and I will update my code.
 *  Warranty   : Absolutely None
 *  
 *  This header also applies to all previous commits. But, I reserve the right to modify this in the future.
 */

#include "sparkfun_log.h"
#include <avr/pgmspace.h>

#define TEST(x)

/* Custom function because the sprintf uses up a TON of memory and
 * it is incomplete on the Arduino platform.
 */
void toString(long value, int* index, char* string)
{
  char count = 0;
  char looper = 0;
  char array[10];

  /* If the value is negative put a negative sign in the output string then
   * invert the number.
   */
  if (value < 0)
  {
    string[(*index)] = '-';
    value *= -1;
    (*index)++;
  }
  if (value == 0)
  {
    array[count] = value % 10 + '0';
    count++;
  }
  /* This will make a string representation of the value
   * in reverse digit order.
   */
  while (value > 0)
  {
    array[count] = value % 10 + '0';
    value = value / 10;
    count++;
  }
  /* Since the above code generates the number in reverse... copy
   * from the tail of the array into the next spot on the output
   * string.
   */
  for (looper = count-1; looper >= 0; looper--)
  {
    string[(*index)] = array[looper];
    (*index)++;
  }
}

/* String copy function. This is to read strings from PROGMEM. */

void strCopy(const char* value, int* index, char* string)
{
  int v_index = 0;
  int len = strlen_P(value);
  for (v_index = 0; v_index < len; v_index++)
  {
    string[(*index)] = pgm_read_byte_near(value + v_index);
    (*index)++;
  }   
}

void createHeader(const char* publicKey, const char* privateKey, int* index, char* string)
{
  strCopy(preamble,       index, string);
  strCopy(publicKey,      index, string);
  strCopy(privateKeyText, index, string);
  strCopy(privateKey,     index, string);
}

void addBodyText(const char* bodyString, long value, int* index, char* string)
{
  strCopy(bodyString, index, string);
  toString(value,     index, string);
}


void createLoggingString(const char* publicKey, const char* privateKey, 
                         long* dataArray, const char** stringArray, char numValues, 
                         char* logString)
{
  int ls_index = 0;
  char i;

  createHeader(publicKey, privateKey, &ls_index, logString);
  
  for (i = 0; i < numValues; i++)
  {
    addBodyText(stringArray[i], dataArray[i], &ls_index, logString);
  }

  logString[ls_index] = 0;
}

