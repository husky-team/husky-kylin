#ifndef ROWENCODER
#define ROWENCODER

#include <iostream>
#include <string>
#include <typeinfo>
#include <vector>
#include <string.h>


#include "string_encoder.hpp"

#include "base/log.hpp"
#include "core/engine.hpp"
#include "io/input/inputformat_store.hpp"
#include "lib/aggregator_factory.hpp"

#include "boost/tokenizer.hpp"

class RowKey{
public:
	StringEncoder encoder;
    std::vector<unsigned char> encoded_data;
	std::vector<std::string> original_data;
	int dimensions_num;
	std::vector<int> offset;



	RowKey(){
		this->encoded_data.clear();
		this->original_data.clear();
		this->dimensions_num = 0;
		this->encoder = StringEncoder();
	}



	RowKey(std::vector<std::string> data, int length){
        this->encoder = StringEncoder();
		this->load_data(data, length);
	}


	void load_data(std::vector<std::string> data, int length){
		this->original_data = data;
		this->encoded_data.clear();
		this->dimensions_num = length;
	}

    void fill_data(std::vector<unsigned char>& destination, const std::vector<unsigned char>& source){
        for(int i=0; i<source.size(); i++)
            destination.push_back(source[i]);
    }


	void encode(){
		this->encoded_data.clear();
		int end_offset = 0;
		this->offset.push_back(end_offset);
        std::vector<unsigned char> temp_uc;
        for(int i=0; i<this->dimensions_num; i++){
			temp_uc.clear();
			temp_uc = this->encoder.encode(this->original_data[i]);
			end_offset += temp_uc.size();
			this->offset.push_back(end_offset);
            this->fill_data(this->encoded_data, temp_uc);
		}
		this->original_data.clear();
        temp_uc.clear();
	}


    std::vector<unsigned char> get_encoded_data(){
		return this->encoded_data;
	}


	void decode(int index, int* i){
		int start = this->offset[index];
		int end = this->offset[index+1];
		int size = end -start;
		this->encoder.decode(this->encoded_data, i, start, size);
	}

	void decode(int index, std::string* i){
        int start = this->offset[index];
		int end = this->offset[index+1];
		int size = end -start;
		this->encoder.decode(this->encoded_data, i, start, size);
	}


	void decode(int index, double* i){
		int start = this->offset[index];
		int end = this->offset[index+1];
		int size = end -start;
		this->encoder.decode(this->encoded_data, i, start, size);
	}

	void decode(int index, float* i){
		int start = this->offset[index];
		int end = this->offset[index+1];
		int size = end -start;
		this->encoder.decode(this->encoded_data, i, start, size);
	}
};


#endif
