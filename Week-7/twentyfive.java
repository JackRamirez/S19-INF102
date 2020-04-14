import java.util.*;
import java.util.Map.Entry;
import java.io.*;
import java.nio.file.*;
import java.lang.*;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.sql.*;




public class twentyfive{
    
    interface Lambda{
        List<String> execute_words(String m);
    }
    
    public static void create_db_schema(Connection con){
        try{
        Statement statement = con.createStatement();
        statement.setQueryTimeout(30);
        
        statement.executeUpdate("CREATE TABLE documents (id INTEGER PRIMARY KEY AUTOINCREMENT, name)");
        statement.executeUpdate("CREATE TABLE words (id, doc_id, value)");
        statement.executeUpdate("CREATE TABLE characters (id, word_id, value)");
        con.close();
        }catch(SQLException e){
            System.err.println(e.getMessage());
        }
                
    }
    
    public static void load_file_into_database(String path_to_file, Connection con){
        
        Lambda func = (str) -> {
                        String fileName = str;
                        String contents = "";
                        try{
                             contents = new String(Files.readAllBytes(Paths.get(fileName)));
                        }catch(IOException e){
                            e.printStackTrace();
                        }                        
                        String words = contents.replaceAll("[^a-zA-Z ]", " ").toLowerCase();
                        String[] words2 = words.split(" ");
                        List<String> wordList = new LinkedList<String>(Arrays.asList(words2));
                        String fileName2 = "../stop_words.txt";
                        String contents2 = "";
                        try{
                             contents2 = new String(Files.readAllBytes(Paths.get(fileName2)));
                        }catch(IOException e){
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
                };
        try{
        List<String> wordList = func.execute_words(path_to_file);
        Statement statement = con.createStatement();
        PreparedStatement pstmt = null;
        pstmt = con.prepareStatement("INSERT INTO documents (name) VALUES (?)");
        pstmt.setString(1, path_to_file);
        pstmt.executeUpdate();
        pstmt = con.prepareStatement("SELECT id from documents WHERE name = ?");
        pstmt.setString(1, path_to_file);
        ResultSet rs = pstmt.executeQuery();
        int doc_id = rs.getInt(1);
        ResultSet rowSet = statement.executeQuery("SELECT MAX(id) FROM words");

        int word_id = rowSet.getInt(1);
        word_id = 0;
        for(String w: wordList){
            if("".equals(w)) continue;
            pstmt = con.prepareStatement("INSERT INTO words VALUES (?, ?, ?)");
            pstmt.setInt(1, word_id);
            pstmt.setInt(2, doc_id);
            pstmt.setString(3, w);
            pstmt.executeUpdate();
            /**
             * Slows down database creation considerably (>20 minutes)
             */
//            int char_id = 0;
//            for(int i  = 0; i < w.length(); i++){
//                char c = w.charAt(i);
//                pstmt = con.prepareStatement("INSERT INTO characters VALUES (?, ?, ?)");
//                pstmt.setInt(1, char_id);
//                pstmt.setInt(2, word_id);
//                pstmt.setString(3, Character.toString(c));
//                pstmt.executeUpdate();
//                char_id += 1;
//            }
            word_id += 1;
        }
        con.close();

        }catch(SQLException e){
            System.err.println("2: " + e.getMessage());
        }  
  
    }
    
    public static void main(String[] args){
        Connection con = null;
        File file = new File ("database.db");

        try{
            
            if(!file.exists()){
                con = DriverManager.getConnection("jdbc:sqlite:database.db");
                create_db_schema(con);
                con = DriverManager.getConnection("jdbc:sqlite:database.db");
                load_file_into_database(args[0], con);
            }
                con = DriverManager.getConnection("jdbc:sqlite:database.db");
                Statement state = con.createStatement();
                ResultSet rs = state.executeQuery("SELECT value, COUNT(*) as C FROM words GROUP BY value ORDER BY C DESC");

                int  i = 0;
                while(rs.next()){
                    if(i == 25) break;
                    String word = rs.getString(1);
                    int count = rs.getInt(2);
                    System.out.println(word + " - " + count);
                    i++;
                }

            con.close();
        }catch(SQLException e){
            System.err.println("3: " + e.getMessage());
        }
    }
}