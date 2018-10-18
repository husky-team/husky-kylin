#ifndef TABLE
#define TABLE

#include "base/log.hpp"
#include "core/engine.hpp"
#include "io/input/inputformat_store.hpp"
#include "lib/aggregator_factory.hpp"

#include "boost/tokenizer.hpp"

#include "row_key.hpp"

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>




class Table{
public:
	std::vector<std::string> raw_data;
	int row_num;
	int column_num;
	std::vector<RowKey*> data;
    char delimiter;
	Table(){
		this->raw_data.clear();
		this->row_num = 0;
		this->column_num = 0;
		this->data.clear();
        this->delimiter = char(25);
	}


    void set_delimiter(char d){
        this->delimiter = d;
    }


	void read_data(){
		auto& infmt =  husky::io::InputFormatStore::create_line_inputformat();
		infmt.set_input(husky::Context::get_param("input"));
		
    	auto read_line = [&](boost::string_ref& line) {
	        std::string str(line);
	        this->raw_data.push_back(str);
	        this->row_num++;
	    };

	    husky::load(infmt, read_line);
	    //husky::LOG_I<<"in the read_data, the size of raw data is "<<this->raw_data.size();
    }
/*
	void create_column_key(){
		//int column_num = 
	}
*/	


	void delete_raw_data(){
		this->raw_data.clear();
	}


	void cut(){
        if(this->row_num == 0){
            husky::LOG_I << "row_num = 0 in the cut";
            return;
        }
		std::stringstream linestr;
		std::vector<std::string> line_data;
		std::string tok;
		for(int i=0; i<this->raw_data.size(); i++){
			linestr = std::stringstream(this->raw_data[i]);
			line_data.clear();
			tok.clear();
            //husky::LOG_I << "in the for loop";
			while(std::getline(linestr, tok, char(1))){
                //husky::LOG_I << "in the while loop";
                //husky::LOG_I<<"token: " << tok;
				line_data.push_back(tok);
			}
            // this->column_num = line_data.size();
            //husky::LOG_I<< "token size is "<<line_data.size();
			RowKey* line_seperated_key = new RowKey(line_data, line_data.size()); 
			this->data.push_back(line_seperated_key);
		}
        this->column_num = this->data[0]->original_data.size();
        husky::LOG_I<<"finish cut";
        husky::LOG_I<<"column_num: "<<this->column_num;
        husky::LOG_I<<"row_num "<<this->row_num;
        husky::LOG_I<<"The original data size is "<<this->data[0]->original_data.size();
	}

    void encode(){
        for(int i=0; i<this->data.size(); i++){
            this->data[i]->encode();
        }
        husky::LOG_I << "finish encode";
    }

    void cut_and_encode(){
        this->cut();
        this->encode();
    }


    bool validate(int row, int column){
        if(this->row_num == 0){
            husky::LOG_I << "false for row_num = 0";
            return false;
        }
        else if(row >= this->row_num){
            husky::LOG_I << "false for row >= row_num";
            return false;
        }
        else if(column >= this->column_num){
            husky::LOG_I << "false for column >= column_num";
            return false;
        }
        else if(row < 0){
            husky::LOG_I << "false for row < 0";
            return false;
        }
        else if(column < 0){
            husky::LOG_I << "false for column < 0";
            return false;
        }
        
        return true;
    }

	void get_data(int row, int column, std::string* niche){
	    husky::LOG_I << "in the get_data";
        if(!this->validate(row,column)){
            niche = NULL;
            husky::LOG_I<<"invalid";
            husky::LOG_I<<"The size of rowkey is "<<this->data.size();
            return;
        }
    
        this->data[row]->decode(column, niche);
	}

	void get_data(int row, int column, int* niche){
        if(!this->validate(row, column)){
            niche = NULL;
            return;
        }

		this->data[row]->decode(column, niche);
	}

	void get_data(int row, int column, float* niche){
        if(!validate(row,  column)){
            niche = NULL;
            return;
        }
        this->data[row]->decode(column, niche);
	}

	void get_data(int row, int column, double* niche){
        if(!validate(row,column)){
            niche = NULL;
            return;
        }
        this->data[row]->decode(column, niche);
	}







};







#endif


