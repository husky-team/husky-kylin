#ifndef ROWENCODER
#define ROWENCODER

#include <iostream>
#include <string>
#include <typeinfo>
#include <vector>
#include <string.h>


#include "string_encoder.hpp"



class RowKey{
public:
	StringEncoder* encoder;
	unsigned char* encoded_data;
	std::vector<std::string> original_data;
	int dimensions_num;
	std::vector<int> offset;



	RowKey(){
		this->encoded_data = NULL;
		this->original_data.clear();
		this->dimensions_num = 0;
		//this->encoder = NULL;
		this->encoder = new StringEncoder();
	}


	~RowKey(){
		//std::cout<<this->encoded_data<<std::endl;
		//std::cout<<"Be destructed"<<std::endl;
	}


	RowKey(std::vector<std::string> data, int length){
		this->load_data(data, length);
	}


	void load_data(std::vector<std::string> data, int length){
		this->original_data = data;
		//this->selectEncoder(data[0]);
		this->encoded_data = NULL;
		this->dimensions_num = length;
		//this->encoder = new StringEncoder();
	}


	// to do: delete or modify
	void selectEncoder(int example){
		/*
		if(typeid(example) == typeid(int)){
			this->encoder = new IntEncoder();
			//std::cout<<"here"<<std::endl;
		}
		else if(typeid(example) == typeid(float))
			this->encoder = new FloatEncoder();
		else if(typeid(example) == typeid(double))
			this->encoder = new DoubleEncoder();
		else if(typeid(example) == typeid(std::string))
			this->encoder = new StringEncoder();
		*/
	}


	void encode(){
		int len = 0;
		for(int i=0; i<this->dimensions_num; i++)
			len += this->original_data[i].length();
		//std::cout<<len<<std::endl;
		this->encoded_data = new unsigned char[len+1];
		//this->encoder->print();
		//int j;
		//std::cout<<this->original_data[0]<<std::endl;
		
		unsigned char* uc = NULL;
		int end_offset = 0;
		this->offset.push_back(end_offset);
		for(int i=0; i<this->dimensions_num; i++){
			uc = NULL;
			uc = this->encoder->encode(this->original_data[i]);
			//std::cout<<"uc: "<<uc[0]<<std::endl;
			end_offset += this->original_data[i].size();
			strcat((char*)this->encoded_data, (char*)uc);
			//std::cout<<this->encoded_data<<std::endl;
			this->offset.push_back(end_offset);
			//std::cout<<"offset: "<<end_offset<<std::endl;

			//this->encoder->decode(&j);
			//std::cout<<j<<std::endl;
		}
		this->original_data.clear();
		//std::cout<<this->encoded_data<<" be encoded"<<std::endl;
	}


	unsigned char* getEncodedData(){
		//std::cout<<this->encoded_data[8]<<" in getEncodedData"<<std::endl;
		return this->encoded_data;
		//return c;
	}


	void decode(int index, int* i){
		int start = this->offset[index];
		int end = this->offset[index+1];
		int size = end -start;
		this->encoder->decode(this->encoded_data, i, start, size);
	}

	void decode(int index, std::string* i){
        std::cout<<"in the decode\n";
        int start = this->offset[index];
		int end = this->offset[index+1];
		int size = end -start;
		this->encoder->decode(this->encoded_data, i, start, size);
	}


	void decode(int index, double* i){
		int start = this->offset[index];
		int end = this->offset[index+1];
		int size = end -start;
		this->encoder->decode(this->encoded_data, i, start, size);
	}

	void decode(int index, float* i){
		int start = this->offset[index];
		int end = this->offset[index+1];
		int size = end -start;
		this->encoder->decode(this->encoded_data, i, start, size);
	}
};


#endif
