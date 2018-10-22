#ifndef STRINGENCODER
#define STRINGENCODER
#include <string>
#include <iostream>
#include <sstream>
#include <string.h>
#include <vector>


class StringEncoder{
public:
	StringEncoder(){
		;
	}


	~StringEncoder(){
		;
	}

    void encode(const std::string& value, std::vector<unsigned char>& output, int outputOffset){
        unsigned char* encoded_data = new unsigned char[value.size()];
        memcpy(encoded_data, value.c_str(), value.size());
        for(int i=0; i<value.size(); i++)
            output[i+outputOffset] = encoded_data[i];
        delete[] encoded_data;
    }


    std::vector<unsigned char> encode(std::string original_data){
        std::vector<unsigned char> encoded_data;
        unsigned char* temp_uc = new unsigned char[original_data.size()];
		memcpy(temp_uc, original_data.c_str(), original_data.size());
        for(int i=0; i<original_data.size(); i++)
            encoded_data.push_back(temp_uc[i]);
        delete[] temp_uc;
		return encoded_data;
	}


	void decode(const std::vector<unsigned char>& c, std::string* s){
        unsigned char* temp_uc = new unsigned char[c.size()];
        for(int i=0; i<c.size(); i++)
            temp_uc[i] = c[i];
		memcpy(s, temp_uc, strlen((char*)temp_uc));
        delete[] temp_uc;
	}

	void decode(const std::vector<unsigned char>& c, int* i){
		std::string temp;
		this->decode(c, &temp);
		std::stringstream ss(temp);
		ss >> *i;
	}

	void decode(const std::vector<unsigned char>& c, double* d){
		std::string temp;
		this->decode(c, &temp);
		std::stringstream ss(temp);
		ss >> *d;
	}

	void decode(const std::vector<unsigned char>& c, float* f){
		std::string temp;
		this->decode(c, &temp);
		std::stringstream ss(temp);
		ss >> *f;
	}

	void decode(std::vector<unsigned char>& c, std::string* s, int offset, int size){
		char* temp_c = new char[size];
        unsigned char* temp_uc = new unsigned char[size];
        for(int i=0; i<size; i++)
            temp_uc[i] = c[offset+i];
		memcpy(temp_c, temp_uc, size);
		*s = std::string(temp_c);
        delete[] temp_c;
        delete[] temp_uc;
	}

	void decode(std::vector<unsigned char>& c, int* i, int offset, int size){
		std::string temp;
		this->decode(c, &temp, offset, size);
		std::stringstream ss(temp);
		ss >> *i;
	}

	void decode(std::vector<unsigned char>& c, double* d, int offset, int size){
		std::string temp;
		this->decode(c, &temp, offset, size);
		std::stringstream ss(temp);
		ss >> *d;
	}

	void decode(std::vector<unsigned char>& c, float* f, int offset, int size){
		std::string temp;
		this->decode(c, &temp, offset, size);
		std::stringstream ss(temp);
		ss >> *f;
	}


};

#endif
