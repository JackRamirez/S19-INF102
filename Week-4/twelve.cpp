#include <iostream>
#include <unordered_map>
#include <stdio.h>
#include <string>
#include <regex>
#include <fstream>
#include <vector>
#include <initializer_list>
#include <functional>
#include <algorithm>

#define me data_storage_obj["data"].first
//#define me stop_words_obj["stop_words"].first

typedef std::vector<std::string> vecType;
typedef std::vector<std::pair<std::string,int>> vecTypeP; 
typedef std::unordered_map<std::string, int> mapType;


typedef std::pair<mapType, std::function<mapType(mapType, std::string)>> pair;
typedef std::unordered_map<std::string, pair> mapTypeF;

typedef std::pair<vecTypeP, std::function<vecTypeP(std::string, int)>> pairV;
typedef std::unordered_map<std::string, pairV> mapTypeFV;


mapType extract_words(mapType& obj, std::string file_path){
    std::ifstream file(file_path);
    std::string data2 ( (std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
    file.close();
    std::transform(data2.begin(),data2.end(),data2.begin(), ::tolower); 
    std::replace_if(data2.begin() , data2.end(), [] (const char& c) { return std::ispunct(c) ;},' ');
    std::istringstream iss(data2);
    vecType results(std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>());
    for (auto t : results) {
        obj[t]++;
    }   
    
    return obj;
    
}

mapType load_stop_words(mapType& obj, std::string pass){
    std::ifstream file("../stop_words.txt");
    std::string r;
    vecType stopWords;
    while(std::getline(file, r, ',')){
        stopWords.push_back(r);
    }
    for(int i = 97; i < 123; i++){
        stopWords.push_back(std::string(1, (char)i ));
    }
    file.close();
    for (auto t : stopWords) {
        obj[t] = 0;
    }  
    mapType m;
    return m;

}

vecTypeP increment_count(vecTypeP& obj, std::string m, int w){
        vecTypeP x;
        obj.insert(obj.end(), std::pair<std::string, int>(m, w));
        return x;
}



int main(int argc, char** argv)
{
        
    mapTypeF data_storage_obj({         //just a filler function
        {"data", {{}, {[&](mapType m, std::string path){ return extract_words(data_storage_obj["data"].first, path); }}}},
        {"init", {{}, {[&](mapType m, std::string path){ return extract_words(data_storage_obj["data"].first, path);}}}},
        {"words", {{}, {[&](mapType m, std::string s){ return data_storage_obj["data"].first; }}}}
    });
    
    mapTypeF stop_words_obj({         //just a filler function
        {"stop_words", {{}, {[&](mapType m, std::string path){ return load_stop_words(stop_words_obj["stop_words"].first, path); }}}},
        {"init", {{}, {[&](mapType m, std::string path){ return load_stop_words(stop_words_obj["stop_words"].first, path);}}}},
        {"is_stop_word", {{}, {[&](mapType m, std::string s){
                                    auto search = stop_words_obj["stop_words"].first.find(s);
                                    if(search == stop_words_obj["stop_words"].first.end()){
                                        return m;
                                    }else{
                                        m["one"] = 1;
                                    return m ;} }}}}
    });
     mapTypeFV word_freqs_obj({         //just a filler function
        {"freqs", {{}, {[&](std::string m, int i){ return increment_count(word_freqs_obj["freqs"].first, m, i); }}}},
        {"increment_count", {{}, {[&](std::string m, int i){ return increment_count(word_freqs_obj["freqs"].first, m, i);}}}},
        {"sorted", {{}, {[&](std::string m, int i){
                             std::sort(word_freqs_obj["freqs"].first.begin(),word_freqs_obj["freqs"].first.end(),[&](const std::pair<std::string,int>&l, const std::pair<std::string,int>& r){
                            if(l.second != r.second)
                                return l.second > r.second;
                            return l.first > r.first;
                        });
                            return word_freqs_obj["freqs"].first; }}}}
    });
 
    mapType m, m2, m3;
    vecTypeP v;
    data_storage_obj["init"].second(m, argv[1]);
    stop_words_obj["init"].second(m, "test");


    for(auto &i: data_storage_obj["words"].second(m3, "test")){ 
       if(stop_words_obj["is_stop_word"].second(m2, i.first).empty()){
            word_freqs_obj["increment_count"].second(i.first, i.second);
        }
    }
    

   
   /////////////
    
    word_freqs_obj.insert(std::pair<std::string, pairV>("top25", {
        {}, [&](std::string m, int w){
            vecTypeP x;
             word_freqs_obj["sorted"].second("test", 1);
            for(int i = 0; i < 25 ; i++){
              std::cout << word_freqs_obj["freqs"].first[i].first << " - " << word_freqs_obj["freqs"].first[i].second << std::endl; }
                 return x;
            }
    }));
    
    word_freqs_obj["top25"].second("test", 1);
    
    return 0;
}