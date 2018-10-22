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
	std::vector<RowKey> data;
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
    }


	void delete_raw_data(){
		this->raw_data.clear();
	}


	void cut(){
        if(this->row_num == 0){
            return;
        }
		std::stringstream linestr;
		std::vector<std::string> line_data;
		std::string tok;
		for(int i=0; i<this->raw_data.size(); i++){
			linestr = std::stringstream(this->raw_data[i]);
			line_data.clear();
			tok.clear();
			while(std::getline(linestr, tok, this->delimiter)){
				line_data.push_back(tok);
			}
			RowKey line_seperated_key = RowKey(line_data, line_data.size()); 
			this->data.push_back(line_seperated_key);
		}
	    this->delete_raw_data();
        this->column_num = this->data[0].original_data.size();
    }


    void encode(){
        for(int i=0; i<this->data.size(); i++){
            this->data[i].encode();
        }
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
        if(!this->validate(row,column)){
            niche = NULL;
            return;
        }
        this->data[row].decode(column, niche);
	}

	void get_data(int row, int column, int* niche){
        if(!this->validate(row, column)){
            niche = NULL;
            return;
        }
		this->data[row].decode(column, niche);
	}

	void get_data(int row, int column, float* niche){
        if(!validate(row,  column)){
            niche = NULL;
            return;
        }
        this->data[row].decode(column, niche);
	}

	void get_data(int row, int column, double* niche){
        if(!validate(row,column)){
            niche = NULL;
            return;
        }
        this->data[row].decode(column, niche);
	}

};







#endif


