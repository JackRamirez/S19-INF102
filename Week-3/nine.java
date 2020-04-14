import java.util.*;
import java.util.Map.Entry;
import java.io.*;
import java.nio.file.*;


interface IFunction{
    Object call(Object arg);
}


class ReadFile implements IFunction{
    public Object call(Object arg){
        String fileName = (String)arg;
        String contents = "";
        try{
             contents = new String(Files.readAllBytes(Paths.get(fileName)));
        }
        catch(IOException e){
            e.printStackTrace();
        }
        return contents;
    }
}

class FilterNormalizeSplitChars implements IFunction{
    public Object call(Object arg){
        String data = (String)arg;
        String[] words = data.replaceAll("[^a-zA-Z ]", " ").toLowerCase().split(" ");
        List<String> wordList = new LinkedList<String>(Arrays.asList(words));
        return wordList;
    }
}


class RemoveStopWords implements IFunction{
    public Object call(Object arg){
        @SuppressWarnings("unchecked")
        List<String> wordList = (List<String>)arg;
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
        String[] stop_words = contents.replaceAll("[^a-zA-Z ]", " ").toLowerCase().split(" ");
        List<String> stopwordList = Arrays.asList(stop_words);
        wordList.removeAll(stopwordList);
        return wordList;
        
    }
}

class Frequency implements IFunction{
    public Object call(Object arg){
        @SuppressWarnings("unchecked")

        List<String> data = (List<String>)arg;
        HashMap<String, Integer> map = new HashMap<String, Integer> ();
        for(String s: data){
            if(s.length() < 2) continue;
            if (!map.containsKey(s)) {  
                    map.put(s, 1);
            }
            else {
                int count = map.get(s);
                map.put(s, count + 1);
            }
        } 
        return map;
    }
}



class Sort implements IFunction{
    public Object call(Object arg){
        @SuppressWarnings("unchecked")
        HashMap<String, Integer> map = (HashMap<String,Integer>)arg; 
        Set<Entry<String, Integer>> set = map.entrySet();
        List<Entry<String, Integer>> list = new ArrayList<Entry<String, Integer>>(set);
        Collections.sort( list, new Comparator<Map.Entry<String, Integer>>()
        {
            public int compare( Map.Entry<String, Integer> o1, Map.Entry<String, Integer> o2 )
            {
                return (o2.getValue()).compareTo( o1.getValue() );
            }
        } );

        return list;
    
    }
}

class top_25 implements IFunction{
    public Object call(Object arg){
        @SuppressWarnings("unchecked")
        List<Entry<String, Integer>> list = (List<Entry<String, Integer>>)arg;
        String r = "";
        int k = 0;
        for(Map.Entry<String, Integer> entry:list){
            if(k == 24){r += entry.getKey()+" - "+ entry.getValue(); break;}
            r += entry.getKey()+" - "+ entry.getValue() + "\n";
            k++;
        }
        return r;
            
    }
}

class TheOne {
    private Object value;
   TheOne(Object v){
        value = v;
    }
    public TheOne bind(IFunction func) {
         value = func.call(value);
         return this;
    }
    
    public void printme() {
        System.out.println(value);
    }
  
}

public class nine{
     public static void main(String[] args) {
     TheOne one = new TheOne(args[0]);
     one.bind(new ReadFile()).bind(new FilterNormalizeSplitChars()).bind(new RemoveStopWords()).bind(new Frequency()).bind(new Sort()).bind(new top_25()).printme();
    }
}



