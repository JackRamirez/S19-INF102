#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include<bits/stdc++.h> 



int main(int argc, char** argv)
{
    std::fstream stopWordsFile;
    stopWordsFile.open("../stop_words.txt", std::fstream::in);
    
    std::string r;
    std::vector<std::string> stopWords;
    
    //Adding stop words to stopWords vector
    while(std::getline(stopWordsFile, r, ',')){
        stopWords.push_back(r);
    }
    
    //Adding ascii characters (lowercase) to stopWords vector
    for(int i = 97; i < 123; i++){
        stopWords.push_back(std::string(1, (char)i ));
    }
    stopWordsFile.close();
    
    std::fstream inputFile;
    inputFile.open(argv[1], std::fstream::in); //getting the text file argument
    std::map<std::string, int> words_freq;
    std::string line;
    
    while(std::getline(inputFile, line)){ 
      std::stringstream iss(line);
      std::string word;
      while(iss >> word){ //separates each word in line (by their spaces)
          std::transform(word.begin(),word.end(),word.begin(), ::tolower); //converts each word to lowercase
          std::replace_if(word.begin() , word.end() ,  
                [] (const char& c) { return std::ispunct(c) ;},' '); //replaces each punctuation mark with a space
          std::istringstream iss2(word);
           //separates "words" by their spaces and adds them to results vector (i.e. "there's" -> "there s" -> {there, s})
          std::vector<std::string> results(std::istream_iterator<std::string>{iss2},
                               std::istream_iterator<std::string>());
          for(int k = 0; k < results.size(); k++ ){
            bool found = false;
            if(std::find(stopWords.begin(), stopWords.end(), results[k]) == stopWords.end()){ //if the word in NOT in stopWords vector -> continue
                std::map<std::string,int>::iterator itr;
                    for(itr = words_freq.begin(); itr != words_freq.end(); ++itr){
                        if(results[k] == itr->first){ //if found in words_freq map, increment the word's freq
                            words_freq[results[k]] += 1;
                            found = true;
                            break;
                        }
                    }
                    if(!found){
                        words_freq.insert(std::pair<std::string,int>(results[k],1)); //else add it to the map
                    }
            }
           }

        }
    }
    inputFile.close();
    
    std::vector<std::pair<std::string,int>> vec;
    //add/copy the pair values of words_freq map to the vec vector
    std::copy(words_freq.begin(),words_freq.end(), std::back_inserter<std::vector<std::pair<std::string,int>>>(vec));
    //sort the vector by the pairs' second value (the frequency) in descending order
    std::sort(vec.begin(),vec.end(),[](const std::pair<std::string,int>&l, const std::pair<std::string,int>& r){
        if(l.second != r.second)
            return l.second > r.second;
        return l.first > r.first;
    });
    
    //print out the first 25 values of the sorted vector
    int i = 0;
    for(auto const &pair: vec){
        if(i == 25) break;
        std::cout << pair.first << " - " << pair.second << std::endl;
        i++;
    }
    
      
    
    return 0;
}