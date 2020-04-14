import java.util.*;
import java.io.*;
import java.lang.Thread;
import java.util.Map.Entry;
import java.util.concurrent.*;
import java.nio.file.*;


abstract class ActiveWFObject extends Thread{
    String name;
    LinkedBlockingQueue<Object[]> queue;
    volatile  boolean  stopMe;
    
    public ActiveWFObject(){
        name = this.getClass().getName();
        queue = new LinkedBlockingQueue<>();
        stopMe = false;   
        //start();
    }
    
    
    public void run(){
        while(!stopMe){
           try{
           if(queue.size() != 0){
              // try{
                Object[] message = queue.remove();
                dispatch(message);
                if(message[0] == "die"){
                    stopMe = true;
                }
            }

            }catch(Exception e){
                continue;
            }

        

        }
    }
    
    
    abstract public void dispatch(Object[] message);
    //abstract void dispatch(Object[] message);
    
}


class WordFreqController extends ActiveWFObject{
    DataStorageManager dsm;
    
    @Override
    public void dispatch(Object[] message){
       // System.out.println(this.getClass().getName() + " dispatched " + message[0]);
         //System.out.println(this.getClass().getName() + " dispatched");
        if((String)message[0] == "run"){
            run2(message);
        }
        else if((String)message[0] == "top25"){
            display(message);
        }
    }
   

   
    private void run2(Object[] message){
        dsm = (DataStorageManager)message[1];
        Object[] obj = {"send_word_freq", this};
        twentyeight.send(dsm, obj);
        
    } 
    
    private void display(Object[] message){
        @SuppressWarnings("unchecked")
        List<Entry<String, Integer>> word_freqs = (List<Entry<String, Integer>>)message[1];
        // List<Entry<String, Integer>> list = arg;
        String r = "";
        int k = 0;
        for(Map.Entry<String, Integer> entry:word_freqs){
            if(k == 24){r += entry.getKey()+" - "+ entry.getValue(); break;}
            r += entry.getKey()+" - "+ entry.getValue() + "\n";
            k++;
        }
        System.out.println(r);
        Object[] obj = {"die"};
        twentyeight.send(dsm, obj);
        stopMe = true;
    }
    
    
    
}


class WordFreqsManager extends ActiveWFObject{
    HashMap<String,Integer> word_freqs = new HashMap<>();

    
    @Override
    public void dispatch(Object[] message){
         //   System.out.println(this.getClass().getName() + " dispatched " + message[0]);
        if((String)message[0] == "word"){
            increment_count(message);
        }
        else if((String)message[0] == "top25"){
            top25(message);
        }
    }
    
    private void increment_count(Object[] message){
        String word = (String)message[1];
          //  System.out.println(this.getClass().getName() + " increm " + message[1]);
        if (!word_freqs.containsKey(word)) {
           // System.out.println(word+"1");
             word_freqs.put(word, 1);
        }
        else {
          //  System.out.println(word+"2");
             int count = word_freqs.get(word);
             word_freqs.put(word, count + 1);
        }
    }
    
    private void top25(Object[] message){
        Object recipient = message[1];
        Set<Entry<String, Integer>> set = word_freqs.entrySet();
        List<Entry<String, Integer>> list = new ArrayList<Entry<String, Integer>>(set);
        Collections.sort( list, new Comparator<Map.Entry<String, Integer>>()
        {
            public int compare( Map.Entry<String, Integer> o1, Map.Entry<String, Integer> o2 )
            {
                return (o2.getValue()).compareTo( o1.getValue() );
            }
        } );
        Object[] obj = {"top25", list};
        twentyeight.send(recipient, obj);
               
        
    }
    
    
}


class StopWordManager extends ActiveWFObject{
    List<String> word_list;
    WordFreqsManager word_freqs_manager;
    
    @Override
    public void dispatch(Object[] message){
        //System.out.println("123dispatch");
        // System.out.println(this.getClass().getName() + " dispatched " + message[0]);
        if((String)message[0] == "init"){
            init(message);
        }
        else if((String)message[0] == "filter"){
            filter(message);
        }else{
            twentyeight.send(word_freqs_manager, message);
        }
    }
    

    
    private void init(Object[] message){
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
        word_list = stopwordList;
        
        word_freqs_manager = (WordFreqsManager)message[1];
        
    }
    
    private void filter(Object[] message){
        String word = (String)message[1];

        if(!word_list.contains(word) && !word.isEmpty() &&  word != null){
            //        System.out.println(this.getClass().getName() + " filter " + word);
            Object[] list = {"word", message[1]};
            twentyeight.send(word_freqs_manager, list);
        }
    }
    
    
}


class DataStorageManager extends ActiveWFObject{
    String data;
    StopWordManager stop_word_manager;
    
    @Override
    public void dispatch(Object[] message){
          //      System.out.println(this.getClass().getName() + " dispatched " + message[0]);
         if((String)message[0] == "init"){
             init(message);
         }
         else if((String)message[0] == "send_word_freq"){
             process_words(message);
         }
         else{
             twentyeight.send(stop_word_manager, message);
         }
    }

    
    private void init(Object[] message){
        String path_to_file = (String)message[1];
        stop_word_manager = (StopWordManager)message[2];
          String contents = "";
        try{
             contents = new String(Files.readAllBytes(Paths.get(path_to_file)));
        }
        catch(IOException e){
            e.printStackTrace();
        }
        data = contents.replaceAll("[^a-zA-Z ]", " ").toLowerCase();
        
    }
    
    private void process_words(Object[] message){
        Object recipient = message[1];
       // System.out.println(recipient.getClass().getName() + "dsm");
        String[] words = data.split(" ");
        List<String> wordList = new LinkedList<String>(Arrays.asList(words));
        for(String w : wordList){
            Object[] obj = {"filter", w};
            twentyeight.send(stop_word_manager, obj);
        }
        Object[] obj = {"top25", recipient};
        twentyeight.send(stop_word_manager, obj);
        
    }
    
    
}



public class twentyeight{
    
    public static void send(Object reciever, Object[] message){
        Class cls = reciever.getClass();
        if(reciever instanceof WordFreqsManager){
            ((WordFreqsManager)reciever).queue.add(message);
        }
        else if(reciever instanceof StopWordManager){
            ((StopWordManager)reciever).queue.add(message);
        }
        else if(reciever instanceof DataStorageManager){
            ((DataStorageManager)reciever).queue.add(message);
        }
        else if(reciever instanceof WordFreqController){
            ((WordFreqController)reciever).queue.add(message);
        }
        //reciever.queue.add(message);
      //  System.out.println(message[0] + " sent");
    }
    
    public static void main(String[] args){
        
        WordFreqsManager wfm = new WordFreqsManager();
        
        StopWordManager swm = new StopWordManager();
        Object[] obj = {"init", wfm};
        send(swm, obj);
        
        //System.out.println(swm.queue.peek());
        
        DataStorageManager dsm = new DataStorageManager();
        Object[] obj2 = {"init", args[0], swm};
        send(dsm, obj2);
        
        WordFreqController wfc = new WordFreqController();
        Object[] obj3 = {"run", dsm};
        send(wfc, obj3);
        
               
        Object[] obj4 = {wfm, swm, dsm, wfc};
         for(Object arg: obj4){
            try{
              //  System.out.println(arg.getClass().getName() + "started");
            //    ((ActiveWFObject)arg).join();
               if(arg instanceof WordFreqsManager){
                    ((WordFreqsManager)arg).start();
                }
                else if(arg instanceof StopWordManager){
                    ((StopWordManager)arg).start();
                }
                else if(arg instanceof DataStorageManager){
                    ((DataStorageManager)arg).start();
                }
                else if(arg instanceof WordFreqController){
                    ((WordFreqController)arg).start();
        }
            }
            catch(Exception e){
                e.printStackTrace();
            }
        }
        
        
        for(Object arg: obj4){
            try{
                
            //    ((ActiveWFObject)arg).join();
               if(arg instanceof WordFreqsManager){
                    ((WordFreqsManager)arg).join();
                }
                else if(arg instanceof StopWordManager){
                    ((StopWordManager)arg).join();
                }
                else if(arg instanceof DataStorageManager){
                    ((DataStorageManager)arg).join();
                }
                else if(arg instanceof WordFreqController){
                    ((WordFreqController)arg).join();
        }
            }
            catch(InterruptedException e){
                e.printStackTrace();
            }
        }
        
        
    }
}