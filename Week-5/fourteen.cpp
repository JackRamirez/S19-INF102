#include <iostream>
#include <unordered_map>
#include <stdio.h>
#include <string>
#include <regex>
#include <fstream>
#include <vector>
#include <list>
#include <functional>
#include <algorithm>

typedef std::vector<std::string> vecType;
typedef std::vector<std::pair<std::string,int>> vecTypeP;
typedef std::unordered_map<std::string, int> mapType;


class WordFreqFramework{
    std::list<std::function<void(std::string)>> load_event_handlers;
    std::list<std::function<void()>> dowork_event_handlers;
    std::list<std::function<void()>> end_event_handlers;
    
    public:
    void register_for_load_event(std::function<void(std::string)> handler){
        load_event_handlers.push_back(handler);
    }
    
    void register_for_dowork_event(std::function<void()> handler){
        dowork_event_handlers.push_back(handler);
    }
    
    void register_for_end_event(std::function<void()> handler){
        end_event_handlers.push_back(handler);
    }
    
    void run(std::string path_to_file){
        for(auto h: load_event_handlers){
            h(path_to_file);
        }
        for(auto h: dowork_event_handlers){
            h();
        }
        for(auto h: end_event_handlers){
            h();
        }
    }
    
};


class StopWordFilter{
    
    vecType stop_words;
    public:
    StopWordFilter(WordFreqFramework &wfapp){
        wfapp.register_for_load_event([this](std::string p){return load(p);});
    }
    
    private:
    void load(std::string ignore){
        std::ifstream file("../stop_words.txt");
        std::string r;
        while(std::getline(file, r, ',')){
            stop_words.push_back(r);
        }
        for(int i = 97; i < 123; i++){
            stop_words.push_back(std::string(1, (char)i ));
        }
        file.close();
    }
    
    public:
    bool is_stop_word(std::string word){
        return std::find(stop_words.begin(), stop_words.end(), word) != stop_words.end();
    }
    
};


class DataStorage{
    std::string data = " ";
    StopWordFilter &stop_word_filter;
    std::list<std::function<void(std::string)>> word_event_handlers;
    
    public:
    DataStorage(WordFreqFramework &wfapp, StopWordFilter &stopword_filter) : stop_word_filter(stopword_filter){
        wfapp.register_for_load_event([this](std::string p){return load(p);});
        wfapp.register_for_dowork_event([this](){return produce_words();});
    }
    
    private:
    void load(std::string path_to_file){
        std::ifstream file(path_to_file);
        std::string data2 ( (std::istreambuf_iterator<char>(file)),
                           (std::istreambuf_iterator<char>()));
        file.close();
        std::replace_if(data2.begin() , data2.end(),
                [] (const char& c) { return std::ispunct(c) ;},' ');
         std::transform(data2.begin(),data2.end(),data2.begin(), ::tolower); 
         data = data2;
    }
    
    void produce_words(){
        std::istringstream iss(data);
        vecType results(std::istream_iterator<std::string>{iss},
                               std::istream_iterator<std::string>());                
        for(auto w: results){
            if(!stop_word_filter.is_stop_word(w)){
                for(auto h: word_event_handlers){
                    h(w);
                }
            }
        }
    }
    
    public:
    void register_for_word_event(std::function<void(std::string)> f){
        word_event_handlers.push_back(f);
    }
    
};



class WordFreqCounter{
    mapType word_freq;
    
    public:
    WordFreqCounter(WordFreqFramework &wfapp, DataStorage &data_storage){
        data_storage.register_for_word_event([this](std::string w){return increment_count(w);});
        wfapp.register_for_end_event([this](){return print_freqs();});
    }
    
    private:
    void increment_count(std::string word){
         auto search = word_freq.find(word);
        if(search != word_freq.end()){ 
            word_freq[word] += 1;
            }
          else{
            word_freq.insert(std::pair<std::string,int>(word,1));
          }
    }
    
    void print_freqs(){
        vecTypeP vec;
         std::copy(word_freq.begin(),word_freq.end(), std::back_inserter<std::vector<std::pair<std::string,int>>>(vec));
        std::sort(vec.begin(),vec.end(),[](const std::pair<std::string,int>&l, const std::pair<std::string,int>& r){
            if(l.second != r.second)
                return l.second > r.second;
            return l.first > r.first;
        });
        
        for(int i = 0; i < 25 ; i++){
           std::cout << vec[i].first << " - " << vec[i].second << std::endl; 
        }
    }  
};

////////////////////

class WordsWithZ{
    int count;
    
    public:
    WordsWithZ(WordFreqFramework &wfapp, DataStorage &ds){
        ds.register_for_word_event([this](std::string p){return findZ(p);});
        wfapp.register_for_end_event([this](){return print_num();});
        count = 0;
    }
    
    private:
    void findZ(std::string word){
        std::string z1("z");
        std::string z2("Z");
        
        std::size_t found = word.find(z1);
        std::size_t found2 = word.find(z2);
        
        if (found!=std::string::npos || found2 != std::string::npos){
            count++;
        }
    }
    
    void print_num(){
        std::cout << "Words with 'z': " << count << std::endl;
    }
    
};


int main(int argc, char** argv)
{
    WordFreqFramework wfapp;
    StopWordFilter stop_word_filter(wfapp);
    DataStorage data_storage(wfapp, stop_word_filter);
    WordFreqCounter word_freq_counter(wfapp, data_storage);
    WordsWithZ words_z(wfapp, data_storage);
    wfapp.run(argv[1]);
    
}