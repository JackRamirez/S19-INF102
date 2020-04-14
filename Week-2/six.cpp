#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <unordered_map>
#include <vector>
#include <regex>
#include <algorithm>

int main(int argc, char** argv)
{
    std::ifstream file("../stop_words.txt"), file2(argv[1]);
    std::string data((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>())),  data2((std::istreambuf_iterator<char>(file2)), (std::istreambuf_iterator<char>()));
    std::transform(data2.begin(),data2.end(),data2.begin(), ::tolower);
    std::replace_if(data.begin(), data.end(), [] (const char& c) { return std::ispunct(c) ;},' '); 
    std::replace_if(data2.begin(), data2.end(), [] (const char& c) { return std::ispunct(c) ;},' ');     
    std::istringstream iss(data), iss2(data2);
    std::vector<std::string> results(std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>()),  results2(std::istream_iterator<std::string>{iss2}, std::istream_iterator<std::string>());
    std::unordered_map<std::string, int> map;
    for(auto i = results2.begin(); i != results2.end(); i++){ bool g = true;
        for(auto j = results.begin(); j != results.end(); j++){
            if(*i == *j || (*i).size() == 1){ g = false; break;}}
        if(g){ if(map.find(*i) != map.end()){map[*i]++;}
            else{map.insert(std::pair<std::string, int>(*i,1));}} }
    std::vector<std::pair<std::string,int>> vec;
    std::copy(map.begin(),map.end(), std::back_inserter<std::vector<std::pair<std::string,int>>>(vec));
    std::sort(vec.begin(),vec.end(),[](const std::pair<std::string,int>&l, const std::pair<std::string,int>& r){
        if(l.second != r.second) return l.second > r.second; return l.first > r.first; });
    for(int i = 0; i < 25 ; i++){ std::cout << vec[i].first << " - " << vec[i].second << std::endl; }
    return 0;
}