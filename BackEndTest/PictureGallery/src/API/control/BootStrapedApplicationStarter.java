package API.control;

import interfaces.BootStrapedComponent;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.MalformedURLException;
import java.rmi.Naming;
import java.rmi.NotBoundException;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.server.RMIClassLoader;
import java.rmi.server.UnicastRemoteObject;

import org.jconfig.Configuration;
import org.jconfig.ConfigurationManager;

import projects.interfaces.CClient;

import API.interfaces.Application;
import API.interfaces.Client;
import API.interfaces.ServerHandle;
import API.model.RemoteObject;
import API.util.RemoteConfigHelper;
import API.util.RemoteObjectHelper;



/**
 * Wird vom RMIComponentLoader verwendet um die entsprechende Applikation
 * anhängig von der angegebenen <code>codebase</code> und dem
 * <code>compClassName</code> vom Klassenserver zu laden und entsprechend
 * ihrer Anforderungen entweder als Server oder Client Komponente zu starten.
 * @author danny, tobi, franky
 * @since 25.07.2004 16:54:02
 * @date 07.08.2004
 * @version 0.02
 * @stereotype utility
 */
public class BootStrapedApplicationStarter implements BootStrapedComponent {
    /** Referenz auf die zu startende Komponentenklasse. */
    protected Class compClass;

    /** Speicherung der Komponenteneigenschaften. */
    protected RemoteObject component;

    /** Speicherung des Serverzugriffes. */
    protected ServerHandle server= null;
    
	/** Speicherung des Serverzugriffes. */
	protected ServerHandle manager= null;

    private RemoteObjectHelper roh;
    private int valid_auswahl;
    private String auswahl;
    private String category;
    private String[] categorynames; 
	private Configuration configuration;
      
  
  /**
     * Stellt eine Auswahl zwischen min und max zur Verfuegung
     * ,liest von der Konsole User Eingaben ein und prueft auf valide Eingaben
     * @param min auswahl minimum
     * @param max auswahl maximum
     * @param question Die Frage welche gestellt werden soll
         * @return valider Wert innerhalb des g.g. Intervalls oder -1 fuer nicht valide
     */
    private int getIntChoose(int min, int max, String question) {
      // Frage stellen und Eingabe einlesen
      System.out.println(question);
      BufferedReader inB = new BufferedReader(new InputStreamReader(System.in)); //gepufferte wandlung von asci nach unicode
      try {
        auswahl = inB.readLine();
      }
      catch (IOException ex1) {}
      // Umwandeln in Int
      Integer i;
      try {
        i = new Integer(auswahl);
      }
      catch (NumberFormatException ex) {
        //falls sich nicht nach int Umwandeln laesst
        System.out.println("Bitte geben Sie eine Zahl zwischen " + min + " und " +
                           max + " ein!");
        return ( -1);
      }
      valid_auswahl = i.intValue();
      //ist der wert innerhalb des Intervalls ?
      if (valid_auswahl > max || valid_auswahl < min) {
        System.out.println("Bitte geben Sie eine Zahl zwischen " + min + " und " +
                           max + " ein!");
        return ( -1);
      }

      return (valid_auswahl);
    }

    /**
     * ausgabeMenue Menue auf konsole ausgeben
     */
    private void ausgabeMenue() {
      
      
      categorynames = configuration.getCategoryNames();
      System.out.println("+-----Menue---------------");
      for(int i = 0; i<categorynames.length; i++ ){
        System.out.println("| (" + i + ") "+categorynames[i]);
      }
    }
  
  
     
    /**
     * Initialisiert die gewuenschte Komponente.
     * - laden der Klasse vom Klassenserver
     * - starten der Koponente abhängig von ihrem Typ <code>client || server</code>
     * - Registrierung der Komponente
     */
    public void init(String jconfigserverUrl){     
        // TODO Wenn keine Typenzuordnung gefunden wird, muss das Programm
        // die notwendigen Parameter beim User abfragen.
        // TODO jConfig eine entsprechende DTD
        // so dass die Eigenschaften vorher geprüft werden können.
        
        System.out.println("=> BootStrapedApplicationStarter.init() "+jconfigserverUrl);
		//Configuration besorgen
		RemoteConfigHelper rch = new RemoteConfigHelper();
		RemoteConfigHelper.load(jconfigserverUrl,"catalogConfig");
		ConfigurationManager cm = rch.getCM();
		this.configuration = ConfigurationManager.getConfiguration("catalogConfig");
		//konsolenabfrage...        
        ausgabeMenue();
        int menueAuswahl = getIntChoose(0, categorynames.length,"Was soll´s denn sein: ");
        //hier wird der ausgewaehlte service (Categoryname) in category gespeichert 
        //ist kürzer;-) 
        category=categorynames[menueAuswahl];
        //Remoteobject mit Konfig daten fuettern 
        roh = new RemoteObjectHelper(configuration,category) ;
        //zuweisen...
        component= roh.getConfiguratedRemoteObject();
        try {
            compClass = RMIClassLoader.loadClass(
                configuration.getProperty("codebase","",category),
                configuration.getProperty("compClassName","",category));
            System.out.println(
                "BootStrapedApplicationStarter.init() After loading Component Class");
        } catch (MalformedURLException e) {
            e.printStackTrace();
            System.out.println(
                "BootStrapedApplicationStarter.init()" + e.getMessage());
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
            System.out.println(
                "BootStrapedApplicationStarter.init() " + e.getMessage());
        } catch (Exception e) {
            e.printStackTrace();
            System.out.println(
                "BootStrapedApplicationStarter.init() " + e.getMessage());
        }
        if (configuration.getProperty("typ","",category).equals("server")) {
            runServer();
        } else if (configuration.getProperty("typ","",category).equals("client")) {
            runClient();
        }
        this.register(component);
        System.out.println("<= BootStrapedApplicationStarter.init()");
    }

    /**
     * Startet eine Koponente vom Typ Client.
     * - Serverzugriff holen: Naming.lookup(Serverkomponente)
     * - neue Instanz der Komponente: class.newInstance() und Initialisierung dieser
     * - exportieren eines RMIStubs für die gegenseitige Kommunikation
     * - Referenz in den Eigenschaften speichern.
     */
    private void runClient() {
        System.out.println("=> BootStrapedApplicationStarter.runClient()");
        Class clientClass= compClass;
        Application client= null;
        String name= component.getCompName();
        try {
            System.out.println(
                "\tlookup for "
                    + component.getRmiName()
                    + " =  compName >"
                    + name);
            server = (ServerHandle) Naming.lookup(component.getRmiName() + name);
            System.out.println(
                "\tlookup(" + component.getRmiName() + name + " = " + server);
            client = (Application) clientClass.newInstance();
            System.out.println("\ncast des remoteObjects in das remote 'Application' "); 
            Client vtc = (Client) client;            
            System.out.println("\tnew instance = " + client);
            vtc.init(component,server);
            System.out.println("\tCClient.init called");
            UnicastRemoteObject.exportObject(client);
            System.out.println("\texported Object = " + client);
            // setzte Eigenreferenz
            component.setApp(client);
        } catch (Exception e) {
            e.printStackTrace();
            System.out.println(
                "Fehler in BootStrapedApplicationStarter.runClient() > UUHUHUHUHU >> "
                    + e.fillInStackTrace()
                    + "\n"
                    + e.getMessage());
        }
        System.out.println("<= BootStrapedApplicationStarter.runClient()");
    }

    /**
     * Startet eine Komponente vom Typ Server.
     * - Starten einer RMIRegistry
     * - Neuinstanziierung und Initialisierung der Komponente
     * - Einbinden der Komponente in die Registry
     */
    private void runServer() {
    	//TODO registrieren eines Servers an einem laufenden (zentralen) Manager ist nicht implementiert.
   // 	muh
        System.out.println("=> BootStrapedApplicationStarter.runServer()") ;
        System.out.println("component: "+component) ;
        Class serverClass = compClass ;
        Server serverObject = null ;
        String name= component.getCompClassName() ;
        try {
            // Anlegen einer Registry, was aber doch eigentlich nur sinnvoll für einen Manager ist, oder?
            LocateRegistry.createRegistry(Integer.parseInt(component.getPort()));
            System.out.println(
                "\tcreateRegistry(Integer.parseInt(rmiPort) ="
                    + component.getPort()
                    + " compClassName >"
                    + name);
            serverObject = (Server) serverClass.newInstance();
            System.out.println("eine neue Instanz von: " + name + " wurde instanziiert");
            
            serverObject.initObjectTable();
            System.out.println("neue object Tabelle");
            System.out.println("\tnew serverInstance " + serverObject);
            Naming.rebind(component.getRmiName() + component.getCompName(), serverObject);
            System.out.println(
                "\nNaming.rebind out > "
                    + component.getRmiName()
                    + "getProp(compName) "
                    + component.getCompName()
                    + " , serverObject ="
                    + serverObject
                    + "\n");
            System.out.println("\n\t Naming.list :") ;
            String[] dfs = Naming.list(component.getRmiName()) ;
            for (int i= 0; i < dfs.length; i++) {
                System.out.println("\n" + dfs[i]);
            }
            server = (ServerHandle) serverObject;
			component.setServerApp(server) ;
//			((Server) serverObject) ;
			// Server-Instanz (den hier gestarteten Server im RemoteObject eintragen)
        } catch (Exception e) {
            e.printStackTrace();
            System.out.println(
                "Fehler in BootStrapedServerStarter.runServer() : "
                    + e.getMessage());
        }
        System.out.println("<= BootStrapedApplicationStarter.runServer()");
    }   
    
    
    /**
     * runService (nicht implementiert)
     * @param remoteObject
     */

    /**
     * Ruft abhängig von Authentifizierungsart die entsprechende 
     * <code>register</code> Methode auf.
     * 
     */
    protected void register(final RemoteObject remoteObject) {
        System.out.println(
            "=> BootStrapedApplicationStarter.register(RemoteObjectProperties "
                + remoteObject
                + " , ServerHandle "
                + server
                + ")");
        //Prüfen der Registermethode
        try {
        	// TODO Diese Ausgabe scheint für Server an Manager nicht zu stimmen 
			System.out.println("\tRegistrierung an Server = " + server);
			System.out.println("\t\t für Komponente = " + component);
			
			//remoteObject
		
            if (remoteObject.getAuthTyp().equals("nothing")) {
                // nothing => keine Anmeldung erforderlich
                System.out.println(
                    "\tKeine Registrierung erforderlich > authTyp = nothing");
            } else if (remoteObject.getAuthTyp().equals("anonym")) {
            	if (remoteObject.getManager().length() == 0) {
           	// vorher statt component remoteObject.getManager() == null, das hat aber spontan nicht mehr funktioniert 
           	// nur an CompName, wenn kein Manager angegeben ist. 
            	
                // anonym => Anmeldung ohne Password            	
                System.out.println(
                    "\tAnonyme Registrierung > authTyp = anonym > an : \n"
                        + server
                        + "\n  > mit : \n"
                        + remoteObject);
                server.register(remoteObject);                
				} else {
					try {
						System.out.println( "Manager == "+component.getManager()+ component.getManagerName());
						manager = (ServerHandle) Naming.lookup(component.getManager() + component.getManagerName());
					} catch (MalformedURLException e1) {
						System.out.println("BootstrapedApp.register: manager lookup faild!!");
						e1.printStackTrace();
					} catch (NotBoundException e1) {
						System.out.println("BootstrapedApp.register: manager lookup faild!!");
						e1.printStackTrace();
					}
					System.out.println("BootstrapedApp.register: manager lookup SUCCESS!!!");
					manager.registerService(remoteObject);				
					System.out.println("BootstrapedApp.register: manager.register SUCCESS!!!");
				}
            } else if (remoteObject.getAuthTyp().equals("password")) {
                // password => Anmeldung mit Password
                System.out.println(
                    "\tRegistrierung mit Authentifikation > authTyp = password");
                server.register(
                    remoteObject,
                    remoteObject.getUsername(),
                    remoteObject.getPasswd());
            } else if (remoteObject.getAuthTyp().equals("admin")) {
                // admin => Anmeldung mit Password und registerAdminClient()
                System.out.println(
                    "\t Admin Client Registrierung > authTyp = admin");
                server.registerAdminClient(
                    remoteObject,
                    remoteObject.getUsername(),
                    remoteObject.getPasswd());
            }
        } catch (RemoteException e) {
            System.out.println(
                "Fehler in BootStrapedApplicationStarter.register(RemoteObjectProperties "
                    + remoteObject
                    + " , ServerHandle "
                    + server
                    + " : "
                    + e.getMessage());
        }
        System.out.println(
            "<= BootStrapedApplicationStarter.register(RemoteObjectProperties "
                + remoteObject
                + " , ServerHandle "
                + server
                + ")");
    }
}
