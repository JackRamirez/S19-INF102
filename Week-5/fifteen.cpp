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
typedef std::pair<std::string,std::string> pairType;
typedef std::function<void(pairType)> func;

class EventManager{
    std::unordered_map<std::string, std::list<func>> subscriptions;
    
    public:
    void subscribe(std::string event_type, func handler){
        auto search = subscriptions.find(event_type);
        if(search != subscriptions.end()){ 
            subscriptions[event_type].push_back(handler);
            }
          else{
              std::list<func> j;
              j.push_back(handler);
            subscriptions.insert(std::pair<std::string,std::list<func>>(event_type, j));
          }
    }
    
    void publish(std::pair<std::string, std::string> p){
        std::string event_type = p.first;
        auto search = subscriptions.find(event_type);
        if(search != subscriptions.end()){
            for(auto h: subscriptions[event_type]){
                    h(p);
            }
        }

    }
    
};



class DataStorage{
    EventManager &em;
    std::string data = " ";
    public:
    DataStorage(EventManager &e_m) : em(e_m){
        em.subscribe("load", [this](pairType p){return load(p);});
        em.subscribe("start", [this](pairType p){return produce_words(p);});
    }
    
    void load(std::pair<std::string,std::string> p){
        std::string path = p.second;
        std::ifstream file(path);
        std::string data2 ( (std::istreambuf_iterator<char>(file)),
                           (std::istreambuf_iterator<char>()));
        file.close();
        std::replace_if(data2.begin() , data2.end(),
                [] (const char& c) { return std::ispunct(c) ;},' ');
         std::transform(data2.begin(),data2.end(),data2.begin(), ::tolower); 
         data = data2;
    }
    
    void produce_words(std::pair<std::string,std::string> p){
        std::istringstream iss(data);
        vecType results(std::istream_iterator<std::string>{iss},
                               std::istream_iterator<std::string>());                
        for(auto w: results){
            em.publish(std::pair<std::string,std::string>("word", w));
        }
        em.publish(std::pair<std::string,std::string>("eof", " "));
    }
       
};

class StopWordFilter{
    EventManager &em;
    vecType stop_words;
  
    public:
    StopWordFilter(EventManager &e_m) : em(e_m){
        em.subscribe("load", [this](pairType p){return load(p); });
        em.subscribe("word", [this](pairType p){return is_stop_word(p);});
    }
    
    void load(std::pair<std::string,std::string> p){
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
    
    void is_stop_word(std::pair<std::string,std::string> p){
        std::string word = p.second;
        if(!(std::find(stop_words.begin(), stop_words.end(), word) != stop_words.end())){
            em.publish(std::pair<std::string,std::string>("valid_word", word));
        }
    }
    
};


class WordFreqCounter{
    mapType word_freq;
    EventManager &em;
    
    public:
    WordFreqCounter(EventManager &e_m) : em(e_m){
        em.subscribe("valid_word", [this](pairType p){return increment_count(p); });
        em.subscribe("print", [this](pairType p){return print_freqs(p);});
    }
    
    void increment_count(std::pair<std::string,std::string> p){
        std::string word = p.second;
          auto search = word_freq.find(word);
        if(search != word_freq.end()){ 
            word_freq[word] += 1;
            }
          else{
            word_freq.insert(std::pair<std::string,int>(word,1));
          }
    }
    
    void print_freqs(std::pair<std::string,std::string> p){
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


class WordFreqApp{
  EventManager &em;
  
  public:
  WordFreqApp(EventManager &e_m): em(e_m){
      em.subscribe("run", [this](pairType p){return run(p);});
      em.subscribe("eof", [this](pairType p){return stop(p);});
  }
  
  void run(std::pair<std::string,std::string> p){
      std::string path = p.second;
      em.publish(std::pair<std::string,std::string>("load", path));
      em.publish(std::pair<std::string,std::string>("start", " "));
  }
  
  void stop(std::pair<std::string,std::string> p){
      em.publish(std::pair<std::string,std::string>("print", " "));
  }
  
  
};

/////////////////////
class WordsWithZ{
    EventManager &em;
    int count;
    
    public:
    WordsWithZ(EventManager &e_m): em(e_m){
        em.subscribe("valid_word", [this](pairType p){return findZ(p);});
        em.subscribe("print", [this](pairType p){return print_num(p);});
        count = 0;
    }
    
    void findZ(pairType word){
        std::string z1("z");
        std::string z2("Z");
        
        std::size_t found = word.second.find(z1);
        std::size_t found2 = word.second.find(z2);
        
        if (found!=std::string::npos || found2 != std::string::npos){
            count++;
        }

    }
    
    void print_num(pairType p){
        std::cout << "Words with 'z': " << count << std::endl;
    }
    
};


int main(int argc, char** argv)
{
    EventManager em;
    DataStorage ds(em);
    StopWordFilter swf(em);
    WordFreqCounter wfc(em);
    WordFreqApp wfa(em);
    WordsWithZ wwz(em);
    em.publish(pairType("run", argv[1]));

}