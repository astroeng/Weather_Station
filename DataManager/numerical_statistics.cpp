/* Derek Schacht
 *  2016 01 07
 *  License : Give me credit where it is due. 
 *  Disclamer : I try and site code that I find on the internet but I am not perfect. If you find 
 *              something that should be sited let me know and I will update my code.
 *  Warranty : Absolutly None
 *  
 *  This header also applies to all previous commits. But, I reserve the right to modify this in the future.
 */

#include "numerical_statistics.h"

#define max(a,b) (a>b?a:b)
#define min(a,b) (a<b?a:b)

Numerical_Statistics::Numerical_Statistics()
{
    /* mean is not reset because it leads to a potentially large
     * jump that makes the stdev compuation less valid.
     */
    meanValue = 0;
    reset();
}

Numerical_Statistics::~Numerical_Statistics()
{

}

void Numerical_Statistics::reset()
{
    maxValue = -0x7fffffff;
    minValue = 0x7fffffff; 
    valueStDev = 0;
    valueCount = 0;
}
	
void Numerical_Statistics::includeValue(long long value)
{
    /* The computation of the mean and the standard deviation was influenced by
     * this stackexchange discussion.
     * http://dsp.stackexchange.com/questions/811/determining-the-mean-and-standard-deviation-in-real-time
     * which itself references
     * Knuth's "Art of Computer Programming" vol 2
     */
    long long previousMean = meanValue;

    valueCount++;

    meanValue += ((value - meanValue) / valueCount);
    valueStDev += ((value - meanValue) * (value - previousMean));

    /* End of code infuenced by said discussion. */

    maxValue = max(value, maxValue);
    minValue = min(value, minValue);

}
long long Numerical_Statistics::getMean()
{
    return meanValue;
}
long long Numerical_Statistics::getStdev()
{
    return valueStDev / max(valueCount,1);
}
long long Numerical_Statistics::getMax()
{
    return maxValue;
}
long long Numerical_Statistics::getMin()
{
    return minValue;
}
long long Numerical_Statistics::getSetSize()
{
    return valueCount;
}
