#ifndef BASEENCODER
#define BASEENCODER

#include <string>	
#include <iostream>


class BaseEncoder{
public:



	BaseEncoder(){
		;
	}


	virtual unsigned char* encode(std::string x){
		return NULL;
	}

	virtual void decode(unsigned char* c, std::string* s){
		;
	}

	virtual void decode(unsigned char* c, int* i){
		;
	}

	virtual void decode(unsigned char* c, double* d){
		;
	}

	virtual void decode(unsigned char* c, float* f){
		;
	}





	~BaseEncoder(){
		;
	}
/*
	unsigned char* getValue(){
		return this->encoded_data;
	}
*/
	virtual void print(){
		std::cout<<"BaseEncoder"<<std::endl;
	}


};


#endif
