import java.util.*;
import java.util.jar.*;
import java.util.Properties;
import java.util.Map.*;
import java.util.stream.Collectors;
import java.io.*;
import java.nio.file.*;
import java.lang.*;
import java.lang.reflect.*; 
import java.net.MalformedURLException; 
import java.net.*;




interface TFWords {
 public List<String> extract_words(String path);
}

interface TFFreqs {
 public List<Entry<String, Integer>> top_25(List<String> words);
}



/***************************************************************************************
*    Title: JarClassLoader
*    Author: John D. Mitchell
*    Date: 1999
*    Code version: 0.5
*    Availability: https://www.javaworld.com/article/2077572/java-tip-70--create-objects-from-jar-files-.html
*
***************************************************************************************/
 class JarClassLoader extends MultiClassLoader
{
    private JarResources  jarResources;
    public JarClassLoader (String jarName)
    {
        // Create the JarResource and suck in the jar file.
        jarResources = new JarResources (jarName);
        }
        protected byte[] loadClassBytes (String className)
        {
        // Support the MultiClassLoader's class name munging facility.
        className = formatClassName (className);
        // Attempt to get the class data from the JarResource.
        return (jarResources.getResource (className));
    }
}
    
    
    
@SuppressWarnings("unchecked")
public class nineteen{
    public static void main(String[] args){

        Properties prop = new Properties();
        String file_name = "config.ini";
        try{
            InputStream is = new FileInputStream(file_name);
            prop.load(is);
        }catch(Exception e){
            System.out.println(e.toString());
        }
        
        try{
 
              JarClassLoader jarLoader = new JarClassLoader(prop.getProperty("words"));
              Class c = jarLoader.loadClass ("words1", true);
              Method method1 = c.getMethod("extract_words", String.class);
              //Object o1 =;
                
              
              JarClassLoader jarLoader2 = new JarClassLoader(prop.getProperty("frequencies"));
              Class<?> c2 = jarLoader2.loadClass ("freq1", true);
              Method method2 = c2.getMethod("top_25", List.class);
              //Object o2 = ;
                                                                                    //top_25                            //extract_words
               List<Entry<String,Integer>> word_freq = (List<Entry<String,Integer>>) method2.invoke(null,(List<String>) method1.invoke(null, args[0]));
                    
                int i  = 0;
                for(Map.Entry<String, Integer> entry: word_freq){
                    if(i == 25) break;
                    System.out.println(entry.getKey() + " - " + entry.getValue());
                    i++;
                }

        }
        catch(InvocationTargetException e){
            System.out.println(e.getCause()) ;   
        
        }catch(Exception e){
            System.out.println(e.toString());
        }
         

    }
}