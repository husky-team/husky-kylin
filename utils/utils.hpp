#include <vector>

std::vector<unsigned char> intToBytes(int paramInt){
	std::vector<unsigned char> arrayOfBytes(4);
	for(int i = 0; i < 4; i++)
		arrayOfBytes[3 - i] = (paramInt >> (i * 8));
	return arrayOfBytes;
}

int bytesToInt(std::vector<unsigned char> bytes){
	return ((bytes[0] << 24) + (bytes[1] << 16) + (bytes[2] << 8) + bytes[3]);
}

std::vector<unsigned char> longToBytes(long paramLong){
	std::vector<unsigned char> arrayOfBytes(4);
	for(int i = 0; i < 4; i++)
		arrayOfBytes[3 - i] = (int)((paramInt >> (i * 8)) & 0xFF);
	return arrayOfBytes;
}

long bytesToLong(std::vector<unsigned char> bytes){
	return ((bytes[0] << 24) + (bytes[1] << 16) + (bytes[2] << 8) + bytes[3]);
}

void writeLong(long num, std::vector<unsigned char> bytes, int offset, int size) {
	for(int i = offset + size - 1; i >= offset; i--) {
		bytes[i] = (unsigned char) num;
		num >>= 8;
	}
}

long readLong(std::vector<unsigned char> bytes, int offset, int size) {
	long integer;
	for (int i = offset, n = offset + size; i < n; i++) {
		integer <<= 8;
		integer |= (long) bytes[i] & 0xFF;
	}
	return integer;
}
