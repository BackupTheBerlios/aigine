package projects.voting.control;

import java.io.File;

import org.jconfig.Configuration;
import org.jconfig.ConfigurationManager;
import org.jconfig.handler.XMLFileHandler;
import org.jconfig.parser.NestedConfigParser;

import projects.voting.model.Vote;
import projects.voting.model.VoteTable;

/**
 * Created on 15.08.2004
 * @author Frank Otto s0503712
 *
 */
public class HelperXmlPersitence {
  
   private Configuration configuration;
  //Vorsicht hier steht auch immer die "general" mit drin!
  private String[] categorynames;
  private String dateiname;
  private VoteTable votes;
  
  
  public HelperXmlPersitence(String _dateiname){ 
        dateiname = _dateiname;
        votes = new VoteTable();
        
        // Nestedconfig parser einstellen
        System.setProperty("jconfig.parser", NestedConfigParser.class.getName());
        // Errorhandler gibt fehlermeldungen auf der console aus
        System.setProperty("jconfig.errorhandler","org.jconfig.error.SimpleErrorHandler");
        // now let the ConfigurationManager read in the nested_demo_config.xml
        configuration = ConfigurationManager.getConfiguration(_dateiname);

        int k = configuration.getNumberOfCategories();
        System.out.println("es gibt " + (k-1) + " Kategorien\n");
        categorynames = configuration.getCategoryNames();
    
        //durch alle Kategorien rennen 
        for (int i = 0; i < categorynames.length; i++) {
          //votes aus der _config.xml ausgeben
          System.out.print("Kategorie: "+categorynames[i]);
          System.out.println(" Anzahl: "+configuration.getIntProperty("count", 0, categorynames[i]));
 
          //da es immer die general Kategorie gibt darf die NICHT mitspielen
          if(categorynames[i] != "general"){
            Vote vote = new Vote();
            vote.setDescription(configuration.getProperty("description","", categorynames[i]));
            vote.setCount(configuration.getIntProperty("count", -1, categorynames[i]));
            votes.put(categorynames[i], vote);
          }
      }

  }
  
  /**
   * speichert die jconfig Configuration in der quellxmldatei
   *
   */
    public void save() {
      //  Nestedconfig parser einstellen
      System.setProperty("jconfig.parser", NestedConfigParser.class.getName());
      // Errorhandler gibt fehlermeldungen auf der console aus
      System.setProperty("jconfig.errorhandler","org.jconfig.error.SimpleErrorHandler");
      // now let the ConfigurationManager read in the nested_demo_config.xml
      final ConfigurationManager cm = ConfigurationManager.getInstance();
      //hier die <count> in der configuration hochsetzen
 
      
      File file = new File(dateiname + "_config.xml");
      XMLFileHandler handler = new XMLFileHandler();
      handler.setFile(file);
      try {
        refreshConfiguration();
        cm.save(handler, configuration);
        System.out.println("file successfully saved");
      } catch (Exception e) {
        System.out.println("!!!Error file not saved!!!");
        e.printStackTrace();
      }
    }
  
  /**
   * Holt die Vote Obj. aus dem Hashtable, liest den count und
   * speichert ihn in der Configuration (jconfig)
   *
   */   
  public void refreshConfiguration(){
    
     for (int i = 0; i < categorynames.length; i++) {
       //votes count aus Hastable in die Configuration updaten    
  
       //da es immer die general Kategorie gibt darf die nicht mitspielen
       if(categorynames[i] != "general"){
         Vote tmpvt = (Vote) votes.get(categorynames[i]);
         configuration.setIntProperty("count", tmpvt.getCount() ,categorynames[i]);
       }
     }
   }
   /**
    * Zuriff auf die Votetable
    * @return VoteTable
    */
   
   public VoteTable getVoteTable(){
     return votes;
   }

}
