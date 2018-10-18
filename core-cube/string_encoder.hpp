#ifndef STRINGENCODER
#define STRINGENCODER
#include "base_encoder.hpp"
#include <string>
#include <iostream>
#include <sstream>
#include <string.h>


class StringEncoder: public BaseEncoder{
public:
	StringEncoder():BaseEncoder(){
		;
	}


	~StringEncoder(){
		;
	}

	unsigned char* encode(std::string original_data){
		unsigned char* encoded_data = new unsigned char[original_data.size()];
		//std::cout<<"size: "<<original_data.size()<<std::endl;
		memcpy(encoded_data, original_data.c_str(), original_data.size());
		//encoded_data = encoded_data+1;
		//std::cout<<"original: "<<original_data<<std::endl;
		//std::cout<<"encode: "<<encoded_data<<std::endl;
		return encoded_data;
	}

	void decode(unsigned char* c, std::string* s){
		//std::cout<<"decode two\n";
		memcpy(s, c, strlen((char*)c));
		//std::cout<<"len: "<<strlen((char*)c)<<std::endl;
		//std::cout<<"finish memcpy in decode two\n";

	}

	void decode(unsigned char* c, int* i){
		std::string temp;
		this->decode(c, &temp);
		std::stringstream ss(temp);
		ss >> *i;
	}

	void decode(unsigned char* c, double* d){
		std::string temp;
		this->decode(c, &temp);
		std::stringstream ss(temp);
		ss >> *d;
	}

	void decode(unsigned char* c, float* f){
		std::string temp;
		this->decode(c, &temp);
		std::stringstream ss(temp);
		ss >> *f;

	}

	void decode(unsigned char* c, std::string* s, int offset, int size){
		//std::cout<<"check\n";
		//std::cout<<"offset: "<<offset<<"    size: "<<size<<std::endl;
		//std::cout<<c[0]<<" in decode two"<<std::endl;
		char* tempc = new char[size];
		//unsigned char* d = c + offset;
		memcpy(tempc, (c+offset), size);
		*s = tempc;
		//std::memcpy(tempc, (c+offset), size);
		//std::cout<<tempc<<" temp in decode 2"<<std::endl;
		//std::cout<<tempc<<" decode 2"<<std::endl;
		//std::cout<<*s<<" decode 2"<<std::endl;
	}

	void decode(unsigned char* c, int* i, int offset, int size){
		std::string temp;
		this->decode(c, &temp, offset, size);
		std::stringstream ss(temp);
		ss >> *i;
	}

	void decode(unsigned char* c, double* d, int offset, int size){
		std::string temp;
		this->decode(c, &temp, offset, size);
		//std::cout<<"decode double: "<<temp<<std::endl;
		std::stringstream ss(temp);
		ss >> *d;
	}

	void decode(unsigned char* c, float* f, int offset, int size){
		std::string temp;
		this->decode(c, &temp, offset, size);
		std::stringstream ss(temp);
		ss >> *f;

	}


};

#endif
