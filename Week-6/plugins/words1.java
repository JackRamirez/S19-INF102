import java.util.*;
import java.util.Map.Entry;
import java.io.*;
import java.nio.file.*;


public class words1{
    
    public static List<String> extract_words(String arg){
        String fileName = arg;
        String contents = "";
        try{
             contents = new String(Files.readAllBytes(Paths.get(fileName)));
        }
        catch(IOException e){
            e.printStackTrace();
        }
        
        String words = contents.replaceAll("[^a-zA-Z ]", " ").toLowerCase();
        
        String[] words2 = words.split(" ");
        List<String> wordList = new LinkedList<String>(Arrays.asList(words2));
        
        String fileName2 = "../stop_words.txt";
        String contents2 = "";
        try{
             contents2 = new String(Files.readAllBytes(Paths.get(fileName2)));
        }
        catch(IOException e){
            e.printStackTrace();
        }
        
        contents2 += " ";
        for(int i = 97; i < 123; i++){
        contents2 += (char)i + " ";
        }
        String[] stop_words = contents2.replaceAll("[^a-zA-Z ]", " ").toLowerCase().split(" ");
        List<String> stopwordList = Arrays.asList(stop_words);
        wordList.removeAll(stopwordList); 
        
        return wordList;
    }
    
    public static void main(String[] args){
            
       // return contents;
       //return wordList;
    }

}