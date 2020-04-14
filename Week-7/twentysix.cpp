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
#include <set>
#include <bits/stdc++.h> 
//#include <boost/any.hpp>

typedef std::vector<std::string> vecType;
//typedef std::vector<std::vector
typedef std::vector<std::pair<std::string,int>> vecTypeP;
typedef std::unordered_map<std::string, int> mapType;
typedef std::pair<std::string,std::string> pairType;
typedef std::function<vecType()> func;
typedef std::pair<std::vector<std::string>, func> pairTypeV;

typedef std::function<vecTypeP()> funcP;
typedef std::pair<vecTypeP, funcP> pairTypeV2;
typedef std::function<std::set<std::string>()> funcS;
typedef std::pair<std::set<std::string>, funcS> pairTypeS;
typedef std::function<mapType()> funcM;
typedef std::pair<mapType, funcM> pairTypeM;




pairTypeV all_words = {{}, nullptr} ;
pairTypeV stop_words =  {{}, nullptr} ;

pairTypeV non_stop_words = { {}, [](){
            vecType v;
            for(auto i = all_words.first.begin(); i != all_words.first.end(); ++i){
                auto search = std::find(stop_words.first.begin(), stop_words.first.end(), *i);
                if(search == stop_words.first.end() && (*i).length() > 1) v.push_back(*i);
                else{ v.push_back(" "); }

            }
//            for(auto i = stop_words.first.begin(); i != stop_words.first.end(); ++i){
//                all_words.first.erase(std::remove(all_words.first.begin(),all_words.first.end(), *i), all_words.first.end());
//            }
            
           return v; 
        }};

pairTypeS unique_words = { {}, []() {
            std::set<std::string> v;
            for(auto i = non_stop_words.first.begin(); i != non_stop_words.first.end(); ++i){
                if((*i) != " ") v.insert(*i);
            }
            return v;
        }};

pairTypeM counts = { {}, []() {
            mapType v;
            /**
             * The commented out for-loop works but has a large complexity.
             * std::count is O(n) combined with a long for-loop through unique_words
             * and we get an output that takes about 10 seconds. (not good performance).
             * 
             * The uncommented for-loop below is less complex but doesn't use unique_words to
             * generate the correct output.
             *  
             */
           // for(auto w: unique_words.first){
               //v[w] = std::count(non_stop_words.first.begin(), non_stop_words.first.end(), w);
            //}
            
            for(auto w: non_stop_words.first){
                auto search = v.find(w);
                if(search != v.end() && w != " "){
                    v[w] ++;
                }
                else{
                    v.insert(std::pair<std::string,int>(w, 1));
                }
            }

            return v;
        }};
        
pairTypeV2 sorted = { {}, []() {
            vecTypeP vec;
             std::copy(counts.first.begin(),counts.first.end(), std::back_inserter<std::vector<std::pair<std::string,int>>>(vec));
        std::sort(vec.begin(),vec.end(),[](const std::pair<std::string,int>&l, const std::pair<std::string,int>& r){
            if(l.second != r.second)
                return l.second > r.second;
            return l.first > r.first;
        });
        return vec;
        }};


//template <typename T, typename T2>
//std::vector<std::pair<T, T2>> all_columns = {all_words, stop_words, non_stop_words, unique_words, counts, sorted};

template <typename T>
void update(T& c){
    //for(auto c: all_columns){
     //   if(c.second != nullptr){
            c.first = c.second();
       // }
    //}
}

int main(int argc, char** argv)
{
    std::ifstream file(argv[1]);
    std::string data2 ( (std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
    file.close();
    std::transform(data2.begin(),data2.end(),data2.begin(), ::tolower); 
    std::replace_if(data2.begin() , data2.end(), [] (const char& c) { return std::ispunct(c) ;},' ');
    std::istringstream iss(data2);
    vecType results(std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>());
    
    all_words.first = results;
    
    std::ifstream file2("../stop_words.txt");
    std::string r;
    vecType stopWords;
    while(std::getline(file2, r, ',')){
        stopWords.push_back(r);
    }
    for(int i = 97; i < 123; i++){
        stopWords.push_back(std::string(1, (char)i ));
    }
    file2.close();
    
    stop_words.first = stopWords;
    
    update(non_stop_words);
    update(unique_words);
    update(counts);
    update(sorted);
    
    int i = 0;
    for(auto pair: sorted.first){
        if(i == 25) break;
        std::cout << pair.first  << " - " << pair.second << std::endl;
        i++;
    }
    
    return 0;
}