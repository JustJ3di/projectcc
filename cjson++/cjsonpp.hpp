#pragma once

#include <string>
#include<unordered_map>
#include<fstream>
#include<vector>
#include<variant>

namespace JSON
{   
    template<typename T>
    union Json_value{
        T data_;
        std::unordered_map<std::string, Json_value> *dict_:
        std::vector<T> vect_;
    };

    template<typename T>
    class cjsonpp
    {
    private:

        Json_value<T> json;
        
        
        std::fstream filename;

        void parse_diz(std::fstream filename){

            char chunk;
            while (true){
                if(chunk == '{'){
                    
                }
            }

        }
        
    public:
        cjsonpp(/* args */);
        ~cjsonpp();
    };
    



} // namespace JSON