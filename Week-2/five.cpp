#include <iostream>
#include <unordered_map>
#include <stdio.h>
#include <string>
#include <regex>
#include <fstream>
#include <vector>
#include <algorithm>

std::string read_file(std::string path_to_file){
    /**
     * Returns a string containing entire file contents.
     */
    std::ifstream file;
    file.open(path_to_file);
    std::string data ( (std::istreambuf_iterator<char>(file)),
                       (std::istreambuf_iterator<char>()));
    file.close();
    return data;
    
}

std::string filter_chars_and_normalize(std::string data){
    /**
     * Transforms entire string into lowercase and 
     * also replaces punctuation marks with a single space.
     */
    std::transform(data.begin(),data.end(),data.begin(), ::tolower); 
    std::replace_if(data.begin() , data.end(),
                [] (const char& c) { return std::ispunct(c) ;},' ');
    return data;

}

std::vector<std::string> scan(std::string data){
    /**
     * Returns a vector composed of each word in the data string.
     */
    std::istringstream iss(data);
    std::vector<std::string> results(std::istream_iterator<std::string>{iss},
                               std::istream_iterator<std::string>());
    return results;
}

std::vector<std::string> remove_stop_words(std::vector<std::string> data){
    /**
     * Removes the stop words in the data vector, and returns the new data vector.
     */
    std::ifstream file("../stop_words.txt");
    std::string r;
    std::vector<std::string> stopWords;
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

    return data;        
}


std::unordered_map<std::string, int> frequency(std::vector<std::string> data){
    /**
     * Returns a map of words and their frequencies.
     */
    std::unordered_map<std::string,int> map;
    for(auto i = data.begin(); i != data.end(); ++i){
        auto search = map.find(*i);
        if(search != map.end()){ 
            map[*i] += 1;
            }
          else{
              map.insert(std::pair<std::string,int>(*i,1));
          }
    }
    return map;
}

std::vector<std::pair<std::string,int>> sort(std::unordered_map<std::string,int> data){
    /**
     * Returns a vector (of pairs) sorted by descending word frequencies.
     */
    std::vector<std::pair<std::string,int>> vec;
    std::copy(data.begin(),data.end(), std::back_inserter<std::vector<std::pair<std::string,int>>>(vec));
    std::sort(vec.begin(),vec.end(),[](const std::pair<std::string,int>&l, const std::pair<std::string,int>& r){
        if(l.second != r.second)
            return l.second > r.second;
        return l.first > r.first;
    });
    
    return vec;
    
}

void print_all(std::vector<std::pair<std::string,int>> data){
    /**
     * Prints the first 25 values of the data vector.
     */
   for(int i = 0; i < 25 ; i++){
       std::cout << data[i].first << " - " << data[i].second << std::endl; 
    }
}


int main(int argc, char** argv)
{
    print_all(sort(frequency(remove_stop_words(scan(filter_chars_and_normalize(read_file(argv[1])))))));
   
    return 0;
}