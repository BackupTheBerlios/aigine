package API.control;

import java.net.MalformedURLException;
import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.server.RMIClassLoader;
import java.rmi.server.UnicastRemoteObject;
import java.util.Properties;
import API.interfaces.Application;
import API.interfaces.Client;
import API.interfaces.ServerHandle;
import API.model.RemoteObject;
import interfaces.BootStrapedComponent;

/**
 * Wird vom RMIComponentLoader verwendet um die entsprechende Applikation
 * anhängig von der angegebenen <code>codebase</code> und dem
 * <code>compClassName</code> vom Klassenserver zu laden und entsprechend
 * ihrer Anforderungen entweder als Server oder Client Komponente zu starten.
 * @author danny, tobi
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

    /**
     * Initialisiert die gewuenschte Komponente.
     * - laden der Klasse vom Klassenserver
     * - starten der Koponente abhängig von ihrem Typ <code>client || server</code>
     * - Registrierung der Komponente
     */
    public void init(final Properties p) {
        // TODO Wenn keine Typenzuordnung gefunden wird, muss das Programm
        // die notwendigen Paramter beim User abfragen.
        // TODO Die Properties mit jConfig in XML speichern und eine entsprechende DTD
        // so dass die Eigenschaften vorher geprüft werden können.
        System.out.println("=> BootStrapedApplicationStarter.init()");
        component= new RemoteObject(p);
        try {
            compClass=
                RMIClassLoader.loadClass(
                    p.getProperty("codebase"),
                    p.getProperty("compClassName"));
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
        if (p.getProperty("typ").equals("server")) {
            runServer();
        } else if (p.getProperty("typ").equals("client")) {
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
            server= (ServerHandle) Naming.lookup(component.getRmiName() + name);
            System.out.println(
                "\tlookup(" + component.getRmiName() + name + " = " + server);
            client= (Application) clientClass.newInstance();
            ((Client) client).init(component, server);
            System.out.println("\tnew instance = " + client);
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
     * Startet eine Koponente vom Typ Server.
     * - Starten einer RMIRegistry
     * - Neuinstanziierung und Initialisierung der Komponente
     * - Einbinden der Komponente in die Registry
     */
    private void runServer() {
        System.out.println("=> BootStrapedApplicationStarter.runServer()");
        Class serverClass= compClass;
        Server serverObject= null;
        String name= component.getCompClassName();
        try {
            LocateRegistry.createRegistry(
                Integer.parseInt(component.getPort()));
            System.out.println(
                "\tcreateRegistry(Integer.parseInt(rmiPort) ="
                    + component.getPort()
                    + " compName >"
                    + name);
            serverObject= (Server) serverClass.newInstance();
            serverObject.initObjectTable();
            System.out.println("\tnew serverInstance " + serverObject);
            Naming.rebind(
                component.getRmiName() + component.getCompName(),
                serverObject);
            System.out.println(
                "\nNaming.rebind out > "
                    + component.getRmiName()
                    + "getProp(compName) "
                    + component.getCompName()
                    + " , serverObject ="
                    + serverObject
                    + "\n");
            System.out.println("\n\t Naming.list :");
            String[] dfs= Naming.list(component.getRmiName());
            for (int i= 0; i < dfs.length; i++) {
                System.out.println("\n" + dfs[i]);
            }
        } catch (Exception e) {
            e.printStackTrace();
            System.out.println(
                "Fehler in BootStrapedServerStarter.runServer() : "
                    + e.getMessage());
        }
        server= (ServerHandle) serverObject;
        // für register Methode erforderlich
        System.out.println("<= BootStrapedApplicationStarter.runServer()");
    }

    /**
     * Ruft abhängig von Authentifizierungsart die entsprechende 
     * <code>register</code> Methode auf.
     */
    protected void register(final RemoteObject remoteObject) {
        System.out.println(
            "=> BootStrapedApplicationStarter.register(RemoteObjectProperties "
                + remoteObject
                + " , ServerHandle "
                + server
                + ")");
        // TODO Prüfen der Registermethode
        try {
			System.out.println("\tRegistrierung an Server = " + server);
			System.out.println("\t\t für Komponente = " + component);
            if (remoteObject.getAuthTyp().equals("nothing")) {
                // nothing => keine Anmeldung erforderlich
                System.out.println(
                    "\tKeine Registrierung erforderlich > authTyp = nothing");
            } else if (remoteObject.getAuthTyp().equals("anonym")) {
                // anonym => Anmeldung ohne Password            	
                System.out.println(
                    "\tAnonyme Registrierung > authTyp = anonym > an : "
                        + server
                        + "  > mit : "
                        + remoteObject);
                server.register(remoteObject);
            } else if (remoteObject.getAuthTyp().equals("password")) {
                // password => Anmeldung mit Password
                System.out.println(
                    "\tRegistrierung mit Authentifikation > authTyp = password");
                server.register(
                    remoteObject,
                    remoteObject.getProps().getProperty("username"),
                    remoteObject.getProps().getProperty("passwd"));
            } else if (remoteObject.getAuthTyp().equals("admin")) {
                // admin => Anmeldung mit Password und registerAdminClient()
                System.out.println(
                    "\t Admin Client Registrierung > authTyp = admin");
                server.registerAdminClient(
                    remoteObject,
                    remoteObject.getProps().getProperty("username"),
                    remoteObject.getProps().getProperty("passwd"));
            }
        } catch (RemoteException e) {
            System.out.println(
                "Fehler inBootStrapedApplicationStarter.register(RemoteObjectProperties "
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
