import java.util.*;
import java.util.Map.Entry;
import java.io.*;
import java.nio.file.*;
import java.lang.*;
import java.lang.reflect.Method; 


@SuppressWarnings("unchecked")

public class seventeen{
    
    public static String read_file(String arg){
        String fileName = arg;
        String contents = "";
        try{
             contents = new String(Files.readAllBytes(Paths.get(fileName)));
        }
        catch(IOException e){
            e.printStackTrace();
        }
        return contents;
    }
    
      public static String filter_chars_and_normalize(String arg){
        String words = arg.replaceAll("[^a-zA-Z ]", " ").toLowerCase(); //.split(" ");
       return words;
    }

    public static List<String> scan(String data){
        String[] words = data.split(" ");
        List<String> wordList = new LinkedList<String>(Arrays.asList(words));
        return wordList;
    }
    
    public static List<String> remove_stop_words(List<String> arg){
       // @SuppressWarnings("unchecked")
        List<String> wordList = arg;
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
    
    
     public static HashMap<String, Integer> frequency(List<String> word_list){
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
         return map;
    }


    
    public static List<Entry<String, Integer>> sort(HashMap<String,Integer> arg){
       // @SuppressWarnings("unchecked")
        HashMap<String, Integer> map = arg; 
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
    
    public static void print_all(List<Entry<String, Integer>> arg){
       // @SuppressWarnings("unchecked")
        List<Entry<String, Integer>> list = arg;
        String r = "";
        int k = 0;
        for(Map.Entry<String, Integer> entry:list){
            if(k == 24){r += entry.getKey()+" - "+ entry.getValue(); break;}
            r += entry.getKey()+" - "+ entry.getValue() + "\n";
            k++;
        }
        System.out.println(r);
            
    }
    
    public static void main(String[] args){

        try{
            Class clazz = Class.forName("seventeen");
            //ClassLoader cLoader = clazz.getClassLoader();

            Method method1 = clazz.getMethod("read_file", String.class);
            //Object o1 = ;

            Method method2 = clazz.getMethod("filter_chars_and_normalize", String.class);
            //Object o2 = ;
            
            Method method3 = clazz.getMethod("scan", String.class);
            //Object o3 = ;
            
            Method method4 = clazz.getMethod("remove_stop_words", List.class);
            //Object o4 = ;
            
            Method method5 = clazz.getMethod("frequency", List.class);
            //Object o5 = ;
            
            Method method6 = clazz.getMethod("sort", HashMap.class);
            //Object o6 = ;
            
            Method method7 = clazz.getMethod("print_all", List.class);
            
            
            method7.invoke(null, (List<Entry<String, Integer>>)method6.invoke(null, (HashMap<String, Integer>)method5.invoke(null, (List<String>)method4.invoke(null, (List<String>)method3.invoke(null, (String)method2.invoke(null, (String)method1.invoke(null, args[0])))))));
            
            
            
            
            
            
        }catch(Exception e){
            System.out.println(e.toString());
        }
      
//        print_all(sort(frequency(remove_stop_words(scan(filter_chars_and_normalize(read_file(args[0])))))));
        
        
    }
}