/* Derek Schacht
 *  2016 01 07
 *  License : Give me credit where it is due. 
 *  Disclamer : I try and site code that I find on the internet but I am not perfect. If you find 
 *              something that should be sited let me know and I will update my code.
 *  Warranty : Absolutly None
 *  
 *  This header also applies to all previous commits. But, I reserve the right to modify this in the future.
 */

#ifndef NUMERICAL_STATISTICS_H
#define NUMERICAL_STATISTICS_H

class Numerical_Statistics
{
public:
	Numerical_Statistics();
	~Numerical_Statistics();
	void reset();
	void includeValue(long long value);
	long long getMean();
	long long getStdev();
	long long getMax();
	long long getMin();
	long long getSetSize();

private:
	long long maxValue;
	long long minValue;
	long long meanValue;
	long long valueStDev;
	long long valueCount;

};

#endif
