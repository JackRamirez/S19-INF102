import java.util.*;
import java.util.Map.Entry;
import java.io.*;
import java.nio.file.*;
import java.lang.*;



public class freq1{
    
    public static List<Entry<String, Integer>> top_25(List<String> word_list){
        HashMap<String, Integer> map = new HashMap<>();
         for(String s: word_list){
                if(s.length() < 2) continue;
                if (!map.containsKey(s)) {  
                        map.put(s, 1);
                }
                else {
                    int count = map.get(s);
                    map.put(s, count + 1);
                }
            } 
        
         Set<Entry<String, Integer>> set = map.entrySet();
        List<Entry<String, Integer>> list = new ArrayList<Entry<String, Integer>>(set);
       Collections.sort( list, (a, b) -> b.getValue().compareTo(a.getValue()));
       
       //new Comparator<Map.Entry<String, Integer>>()
//        {
//            public int compare( Map.Entry<String, Integer> o1, Map.Entry<String, Integer> o2 )
//            {
//                return (o2.getValue()).compareTo( o1.getValue() );
//            }
//        } );
        
        return list;
        
    }
    
    public static void main(String[] args){
        //nothing
    }
}