#include <iostream>
#include <unordered_map>
#include <stdio.h>
#include <string>
#include <regex>
#include <fstream>
#include <vector>
#include <algorithm>

typedef std::vector<std::string> vecType;
typedef std::vector<std::pair<std::string,int>> vecTypeP;
typedef std::unordered_map<std::string, int> mapType;


void no_op(){}

void print_all(std::vector<std::pair<std::string,int>> data, void (*func)()){
 
   for(int i = 0; i < 25 ; i++){
       std::cout << data[i].first << " - " << data[i].second << std::endl; 
    }
    (*func)(); //no_op
}


void sort(std::unordered_map<std::string,int> data, void (*func)(vecTypeP, void(*)())){
    vecTypeP vec;
    std::copy(data.begin(),data.end(), std::back_inserter<std::vector<std::pair<std::string,int>>>(vec));
    std::sort(vec.begin(),vec.end(),[](const std::pair<std::string,int>&l, const std::pair<std::string,int>& r){
        if(l.second != r.second)
            return l.second > r.second;
        return l.first > r.first;
    });
    
    //return vec;
    (*func)(vec, no_op); //print_all
    
}


void frequency(std::vector<std::string> data, void (*func)(mapType, void(*)(vecTypeP, void(*)()))){
    mapType map;
    for(auto i = data.begin(); i != data.end(); ++i){
        auto search = map.find(*i);
        if(search != map.end()){ 
            map[*i] += 1;
            }
          else{
              map.insert(std::pair<std::string,int>(*i,1));
          }
    }
    //return map;
    (*func)(map, print_all); //sort
    
}

                                                        //freq
void remove_stop_words(std::vector<std::string> data, void (*func)(vecType, void(*)(mapType, void(*)(vecTypeP, void(*)())))){
  
    std::ifstream file("../stop_words.txt");
    std::string r;
    vecType stopWords;
    while(std::getline(file, r, ',')){
        stopWords.push_back(r);
    }
    //Adding ascii characters (lowercase) to stopWords vector
    for(int i = 97; i < 123; i++){
        stopWords.push_back(std::string(1, (char)i ));
    }
    file.close();
    
    for(auto i = stopWords.begin(); i != stopWords.end(); ++i){
        data.erase(std::remove(data.begin(),data.end(), *i), data.end());
    }

    //return data; 
    (*func)(data, sort);   //freq
}

                                // 
void scan(std::string data, void (*func)(vecType, void(*)(vecType, void(*)(mapType, void(*)(vecTypeP, void(*)()))))){
   
    std::istringstream iss(data);
    vecType results(std::istream_iterator<std::string>{iss},
                               std::istream_iterator<std::string>());
    //return results;
    (*func)(results, frequency); //remove_stop_words
    
}

                                    //scan                      //no_op
void normalize(std::string data, void (*func)(std::string, void(*)(vecType, void(*)(vecType, void(*)(mapType, void(*)(vecTypeP, void(*)())))))){
    std::transform(data.begin(),data.end(),data.begin(), ::tolower); 
    //return data;
    (*func)(data, remove_stop_words); //scan
}

                                            //noralize          //scan                 //remove_stop_words
void filter_chars(std::string data, void (*func)(std::string, void(*)(std::string, void(*)(vecType, void(*)(vecType, void(*)(mapType, void (*)(vecTypeP, void(*)()))))))){ //normalize
    std::replace_if(data.begin() , data.end(),
                [] (const char& c) { return std::ispunct(c) ;},' ');
    (*func)(data, scan); //normalize
}

                                        //filter_chars               //normalize            //scan          //remove_stop_words            
void read_file(std::string file_path, void (*func)(std::string, void(*)(std::string, void(*)(std::string, void(*)(vecType, void(*)(vecType, void(*)(mapType, void(*)(vecTypeP, void(*)())))))))){ 
    std::ifstream file;
    file.open(file_path);
    std::string data ( (std::istreambuf_iterator<char>(file)),
                       (std::istreambuf_iterator<char>()));
    file.close();
    //return data;
    (*func)(data, normalize); //filter_chars
}



int main(int argc, char** argv)
{
    read_file(argv[1], filter_chars);   
} 