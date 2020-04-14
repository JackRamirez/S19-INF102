#include <iostream>
#include <unordered_map>
#include <stdio.h>
#include <string>
#include <regex>
#include <fstream>
#include <vector>
#include <algorithm>
#include<bits/stdc++.h> 


typedef std::vector<std::string> vecType;
typedef std::vector<std::pair<std::string,int>> vecTypeP;
typedef std::unordered_map<std::string, int> mapType;

class DataStorageManager{
    std::string data = " ";
    public:
    vecType dispatch(std::string message[]){
        if(message[0] == "init"){
            return init(message[1]);       
        }
        else if(message[0]  == "words"){
            return words();
        }
        else{
            throw "Message not understood";
        }
    }
    
    vecType init(std::string file_path){
        std::ifstream file(file_path);
        std::string data2 ( (std::istreambuf_iterator<char>(file)),
                       (std::istreambuf_iterator<char>()));
        file.close();
        std::transform(data2.begin(),data2.end(),data2.begin(), ::tolower); 
        std::replace_if(data2.begin() , data2.end(),
                [] (const char& c) { return std::ispunct(c) ;},' ');
        data = data2;
        vecType vec;
        return vec;
        
    }
    
    vecType words(){
        std::istringstream iss(this->data);
        vecType results(std::istream_iterator<std::string>{iss},
                               std::istream_iterator<std::string>());
        return results;
    }
    
};


class StopWordManager{
    vecType stop_words;
    public:
    
    bool dispatch(std::string message[]){
        if(message[0] == "init"){
            return this->init();
        }
        else if(message[0]=="is_stop_word"){
            return this->is_stop_word(message[1]);
        }
        else{
            throw "Message not understood";
        }
    }
    
    bool init(){
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
        this->stop_words = stopWords;
        return true;
    }
    
    bool is_stop_word(std::string word){
        return std::find(stop_words.begin(), stop_words.end(), word) != stop_words.end();
    }
    
};


class WordFrequencyManager{
    mapType word_freq;
    public:
    
    vecTypeP dispatch(std::string message[]){
        if(message[0] == "increment_count"){
            return increment_count(message[1]);
        }
        else if(message[0] == "sorted"){
            return sorted();
        }
        else{
            throw "Message not understood";
        }
    }
    
    vecTypeP increment_count(std::string word){
        auto search = word_freq.find(word);
        if(search != word_freq.end()){ 
            word_freq[word] += 1;
            }
          else{
            word_freq.insert(std::pair<std::string,int>(word,1));
          }
          vecTypeP vec;
          return vec;
    }
    
    vecTypeP sorted(){
        vecTypeP vec;
         std::copy(word_freq.begin(),word_freq.end(), std::back_inserter<std::vector<std::pair<std::string,int>>>(vec));
        std::sort(vec.begin(),vec.end(),[](const std::pair<std::string,int>&l, const std::pair<std::string,int>& r){
            if(l.second != r.second)
                return l.second > r.second;
            return l.first > r.first;
        });
        return vec;
    }
    
};

class WordFrequencyController{
    DataStorageManager dsm;
    StopWordManager swm;
    WordFrequencyManager wfm;
    public:
    void dispatch(std::string message[]){
        if(message[0] == "init"){
            init(message[1]);
        }
        else if(message[0] == "run"){
            run();
        }
        else{
            throw "Message not understood";
        }
    }
    
    void init(std::string path){
        std::string message[] = {"init", path};
        dsm.dispatch(message);
        std::string message2[] = {"init"};
        swm.dispatch(message2);
    }
    
    void run(){
        std::string message[] = {"words"};
        for(auto i: dsm.dispatch(message)){
            std::string m[] = {"is_stop_word", i};
            if(!swm.dispatch(m)){
                std::string e[] = {"increment_count", i};
                wfm.dispatch(e);
            }
        }
        std::string d[] = {"sorted"};
        vecTypeP word_freq = wfm.dispatch(d);
        for(int i = 0; i < 25 ; i++){
           std::cout << word_freq[i].first << " - " << word_freq[i].second << std::endl; 
        }
    }
    
};



int main(int argc, char** argv){
    WordFrequencyController wfc;
    std::string message[] = {"init", argv[1]};
    wfc.dispatch(message);
    std::string message2[] = {"run"};
    wfc.dispatch(message2);
    return 0;
}
