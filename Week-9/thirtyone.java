import java.util.*;
import java.io.*;
import java.lang.Thread;
import java.util.Map.Entry;
import java.nio.file.*;



//Custom Tuple Type
class Pair{
     String key;
     Integer value;
    
    public Pair(String k, Integer val){
        key = k;
        value = val;
    }
    
    public String getKey(){
        return key;
    }
    
    public Integer getValue(){
        return value;
    }
    
    public void setKey(String k){
        key = k;
    }
    
    public void setValue(Integer v){
        value = v;
    }
    
    
}


public class thirtyone{
    

    interface Lambda1{ List<String> scan(String m); }
    
    interface Lambda2{ List<String> remove_stop_words(List<String> m); }
    
/***************************************************************************************
*    Title: Generator
*    Author: mherrmann (GitHub)
*    Date: Dec 3, 2015
*    Code version: 1.0
*    Availability: https://github.com/mherrmann/java-generator-functions
*
***************************************************************************************/
   
    public static Generator<String> simple;
    
    public static void partition(String data_str, int nlines){
        simple = new Generator<String>(){
            public void run() throws InterruptedException{
                String[] words = data_str.split("\\r?\\n");
                for(int i = 0; i < words.length; i += nlines){
                    String[] words_sub = Arrays.copyOfRange(words, i, i+nlines);
                    List<String> strList = Arrays.asList(words_sub);
                    String joinedString = String.join("\n", strList);
                    yield(joinedString);
                    
                }
            }
        };
        
    }
    
    public static List<Pair> split_words(String data_str){
        
        Lambda1 func1 = (str) -> {
                        String[] stop_words = str.replaceAll("[^a-zA-Z]", " ").toLowerCase().split(" ");
                        return new LinkedList<>(Arrays.asList(stop_words));
                    };
        Lambda2 func2 = (list) -> {
                        String fileName = "../stop_words.txt";
                        String contents = "";
                        try{
                             contents = new String(Files.readAllBytes(Paths.get(fileName)));
                        }
                        catch(IOException e){
                            e.printStackTrace();
                        }
                        contents += " ";
                        for(int i = 97; i < 123; i++){
                        contents += (char)i + " ";
                        }
                        String[] stop_words = contents.replaceAll("[^a-zA-Z]", " ").toLowerCase().split(" ");
                        List<String> stopwordList = Arrays.asList(stop_words);
                        list.removeAll(stopwordList);
                        return list;
                    };
                    
        List<Pair> result = new LinkedList<>();
        
        List<String> words = func2.remove_stop_words(func1.scan(data_str));
        
        for(String w: words){
            if(w.isEmpty()) continue;
            Pair t = new Pair(w, 1);
            result.add(t);
        }
        
        return result;
        
    }
    
    
    
    public static HashMap<String,List<Pair>> regroup(List<List<Pair>> pairs_list){
        HashMap<String,List<Pair>> mapping = new HashMap<>();
        for(List<Pair> pairs: pairs_list){
            for(Pair p: pairs){
                if(mapping.containsKey(p.getKey())){
                    List<Pair> m = mapping.get(p.getKey());
                    m.add(p);
                    mapping.put(p.getKey(),m);
                }
                else{
                    List<Pair> m = new LinkedList<>();
                    m.add(p);
                    mapping.put(p.getKey(), m);
                }
            }
        }
        return mapping;
        
    }
    
    
    public static Pair count_words(Entry<String,List<Pair>> mapping){
        //Pair p = new Pair();
        List<Pair> p = mapping.getValue();
        int count = p.stream().reduce(0, (e1, e2) -> e1 + e2.getValue(), Integer::sum);
        
        return new Pair(mapping.getKey(), count);
        
    }
    
    public static String read_file(String path_to_file){
        String contents = "";
        try{
             contents = new String(Files.readAllBytes(Paths.get(path_to_file)));
        }
        catch(IOException e){
            e.printStackTrace();
        }
        
        return contents;
    }
    
    public static List<Pair> sort(List<Pair> list){
         Collections.sort( list, new Comparator<Pair>()
        {
            public int compare(Pair o1,Pair o2 )
            {
                return (o2.getValue()).compareTo( o1.getValue() );
            }
        } );

        return list;
    }
    
    public static void main(String[] args){
        
    partition(read_file(args[0]), 200);
    
    List<List<Pair>> p = new LinkedList<>();
    
    for(String element: simple){
        p.add(split_words(element));
    }
    
    
    HashMap<String,List<Pair>> m = regroup(p);
    
    
    List<Pair> k = new LinkedList<>();
    
    for(Map.Entry<String, List<Pair>> en: m.entrySet()){
        k.add(count_words(en));
    }
    
    List<Pair> n = sort(k);
    int i = 0;
    for(Pair d: n){
        if(i == 25) break;
        System.out.println(d.getKey() + " - " + d.getValue());
        i++;
    }
    
    }
    
}