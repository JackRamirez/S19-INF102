import java.util.*;
import java.io.*;
import java.lang.*;
import java.util.concurrent.*;
import java.lang.Thread;
import java.util.Map.Entry;
import java.nio.file.*;


class extra_class extends Thread{
    
    public void run(){
        //process_words
        HashMap<String,Integer> word_freqs = new HashMap<>();
        while(true){
            Object word_obj;
            try{
               word_obj = twentynine.word_space.remove();
            }catch(Exception e){ //empty
            //continue;
            break;
            }
            String word = (String)word_obj;
            if(!twentynine.stop_words.contains(word)){
                 if(!word_freqs.containsKey(word)) {
                    //System.out.println(word+"1");
                     word_freqs.put(word, 1);
                }
                else {
                   // System.out.println(word+"2");
                     int count = word_freqs.get(word);
                     //System.out.println(word + ">>" + count);
                     word_freqs.put(word, count + 1);
                }
            }

        }
        
        twentynine.freq_space.add(word_freqs);
    }
}


public class twentynine{
    
    public static LinkedBlockingQueue<Object> word_space = new LinkedBlockingQueue<>();
    public static LinkedBlockingQueue<Object> freq_space = new LinkedBlockingQueue<>();
    
    //stop_words
    public static List<String> create_stop_words(){
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
        return stopwordList;
    }
    
    public static List<String> stop_words = create_stop_words();
    
//    public static void process_words(){
//        HashMap<String,Integer> word_freqs = new HashMap<>();
//        while(true){
//            Object word_obj;
//            try{
//               word_obj = word_space.remove();
//            }catch(Exception e){ //empty
//            
//            break;
//            }
//            String word = (String)word_obj;
//            if(!stop_words.contains(word)){
//                 if(!word_freqs.containsKey(word)) {
//                   // System.out.println(word+"1");
//                     word_freqs.put(word, 1);
//                }
//                else {
//                  //  System.out.println(word+"2");
//                     int count = word_freqs.get(word);
//                     word_freqs.put(word, count + 1);
//                }
//            }
//
//        }
//        
//        freq_space.add(word_freqs);
//    }
    

    
    public static void populate_word_space(String filename){
        String contents = "";
        try{
             contents = new String(Files.readAllBytes(Paths.get(filename)));
        }
        catch(IOException e){
            e.printStackTrace();
        }
        contents = contents.replaceAll("[^a-zA-Z ]", " ").toLowerCase();
        String[] words = contents.split(" ");
        List<String> wordList = new LinkedList<String>(Arrays.asList(words));
        for(String w : wordList){
          if(w.isEmpty() || w == null) continue;
          //System.out.println(w);
          word_space.add(w);  
        }

    }
    
    public static List<extra_class> workers = new LinkedList<>();
    
    
    public static HashMap<String,Integer> word_freqs = new HashMap<>();
    
    
    public static void main(String[] arg){
        
        populate_word_space(arg[0]);
        
        for(int i = 0; i < 5; ++i){
            extra_class obj = new extra_class();
           workers.add(obj);
        }
        
        for(extra_class v: workers){
            v.start();
 

        }
        
        for(extra_class v: workers){
            try{
               // v.sleep(10);
                v.join();
            }catch(Exception e){
                e.printStackTrace();
            }

        }
        
        while(freq_space.size() != 0){
            HashMap<String,Integer> freq_obj = (HashMap<String,Integer>)freq_space.remove();
            for(Map.Entry<String,Integer> pair: freq_obj.entrySet()){
                int count;
                if(word_freqs.containsKey(pair.getKey())){
                    count = freq_obj.get(pair.getKey()) + word_freqs.get(pair.getKey());
                 }
                 else{
                     count = freq_obj.get(pair.getKey());
                 }
                 word_freqs.put(pair.getKey(), count);
            }
        }
        
        
        //List<Entry<String, Integer>> word_freqs = (List<Entry<String, Integer>>)message[1];
        // List<Entry<String, Integer>> list = arg;
        Set<Entry<String, Integer>> set = word_freqs.entrySet();
        List<Entry<String, Integer>> list = new ArrayList<Entry<String, Integer>>(set);
        Collections.sort( list, new Comparator<Map.Entry<String, Integer>>()
        {
            public int compare( Map.Entry<String, Integer> o1, Map.Entry<String, Integer> o2 )
            {
                return (o2.getValue()).compareTo( o1.getValue() );
            }
        } );
        
        String r = "";
        int k = 0;
        for(Map.Entry<String, Integer> entry: list){
            if(k == 24){r += entry.getKey()+" - "+ entry.getValue(); break;}
            r += entry.getKey()+" - "+ entry.getValue() + "\n";
            k++;
        }
        System.out.println(r);
        
    }
}