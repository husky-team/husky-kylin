#include <integerDimEnc.hpp>

IntegerDimEnc::IntegerDimEnc(int len) {
	// if(len <= 0 || len >= CAP.length)
	// 	// throw error
	this->fixedLen = len;
}

/** return the fixed length of encoded bytes */
int IntegerDimEnc::getLengthOfEncoding() {
	return fixedLen;
}
/** encode given value to bytes, note the NULL convention */
void IntegerDimEnc::encode(const std::string& valueStr, std::vector<unsigned char> output, int outputOffset) {
	// if (valueStr.empty()) {}
	long integer = std::stol(valueStr);
	writeLong(integer + CAP[fixedLen], output, outputOffset, fixedLen);
}

/** decode given bytes to value string, note the NULL convention */
std::string IntegerDimEnc::decode(std::vector<unsigned char> bytes, int offset, int len) {
	long integer = readLong(bytes, offset, len) - CAP[fixedLen];

	// only take useful bytes
	integer = integer & MASK[fixedLen];
	bool positive = (integer & ((0x80L) << ((fixedLen - 1) << 3))) == 0;
	if(!positive) {
		integer |= (~MASK[fixedLen]);
	}

	return std::to_string(integer);
}