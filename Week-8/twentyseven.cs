using System;
using System.IO;
using System.Linq;
using System.Collections.Generic;

namespace inf102
{
    class TwentySeven 
    {
        
        public static IEnumerable<char> characters(string filename){
            foreach(string line in File.ReadLines(filename)){
                string line1 = line + '\n';
                foreach(char c in line1){
                    yield return c;
                }
            }
        }
        
        public static IEnumerable<string> all_words(string filename){
            bool start_char = true;
            string word = "";
            foreach(char c in characters(filename)){
                if(start_char){
                    if(char.IsLetter(c)){
                        word = char.ToString(char.ToLower(c));
                        start_char = false;
                    }else{
                        continue;
                    }
                }else{
                    if(char.IsLetter(c)){
                        word = word + char.ToString(char.ToLower(c));
                    }else{
                        start_char = true;
                        yield return word; 
                        word = "";
                    }
                }
            }
        }
        
        public static IEnumerable<string> non_stop_words(string filename){
            string file_text = File.ReadAllText("../stop_words.txt");
            List<string> stop_words = new List<string>();
            string[] stop_words_list = file_text.Split(',');
            foreach(string w in stop_words_list){
                stop_words.Add(w);
            }
            

            for(int i = 97; i < 123; i++){
                stop_words.Add(char.ToString((char)i));
            }

            foreach(string word in all_words(filename)){
                if(!stop_words.Contains(word))
                    yield return word;
            }
        }
        
        public static IEnumerable<IOrderedEnumerable<System.Collections.Generic.KeyValuePair<string,int>>> count_and_sort(string filename){
            Dictionary<string,int> freqs = new Dictionary<string,int>();
            int i = 1;
            foreach(string w in non_stop_words(filename)){
                if(!freqs.ContainsKey(w)){
                    freqs[w] = 1;
                }else{
                    freqs[w]++;
                }
                if((i % 5000) == 0){
                    var items = from pair in freqs
                                orderby pair.Value descending
                                select pair;
                    yield return items;
                }
                i++;
            }
            var items2 = from pair in freqs
                        orderby pair.Value descending
                         select pair;

            yield return items2;
            
        }
        
        static void Main(string[] args) 
        {

            
            foreach(var word_freqs in count_and_sort(args[0])){
                Console.WriteLine("------------------------");
                int i = 0;
                foreach(KeyValuePair<string,int> pair in word_freqs){
                    if(i == 25) break;
                    Console.WriteLine("{0} - {1}", pair.Key, pair.Value);
                    i++;
                }
            }
            
            
        }
    }
}