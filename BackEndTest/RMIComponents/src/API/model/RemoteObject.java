package API.model;

import java.io.Serializable;
import java.util.Properties;
import API.interfaces.Application;

/**
 * Speichert die Eigenschaften einer RMI Komponente (Verbindungsinformationen etc.)
 * @author danny, tobi
 * @since 25.07.2004
 * @version 0.01
 */
public class RemoteObject implements Serializable {
    /** Die vom Loader eingelesenen Properties. */
    protected Properties props;

    /** Beschreibung der Komponente. */
    protected String description;

    /** Typ der Koponente, derzeitig client oder server ... */
    protected String servicetyp;

    /** Classpath für den RMIClassLoader (meist die URL des ClassServers) */
    protected String codebase;

    /**
     * Name des Serverdienstes, auf den zugegriffen werden soll oder
     * unter welchem Namen eine Serverimplemtierung gestartet werden soll.
     * - ist die Komponente ein Client : serverName =  zielServer  bsp1. tweety.de
     * - ist die Komponente ein Server : serverName = aktueller Server bsp2 www.dagnu.de
     */
    protected String hostname;

    /** Name der Klasse für die zu ladene Komponente, z.B. projects.voting.VTServer */
    protected String compClassName;

    /**
     * Name für den Eintrag der Koponente in die Registry <code>rebind(...)</code> bzw.
     * der Name für <code>lookup(...)</code>, je nachdem ob es sich um einen Server
     * oder Client handelt.
     */
    protected String compName;

    /** IPAdresse des jeweiligen Serverdienstes. */
    protected String ipaddr;

    /** Port für die Registry. */
    protected String port;

    /**
     * Name für die Einbindung in die RMI Registry. Wird im Konstruktor gesetzt:
     * <code>rmi://" + hostname  + ":" + port + "/";</code>
     */
    protected String rmiName;

    /**
     * Referenz auf eine Instanz der Application, die mit den Parametern
     * gestartet wurde.
     */
    protected Application app;

    /** Authentifizierungstyp, derzitig anonym, nothing, password */
    protected String authTyp;

    /**
     * Konstruktor nimmt die vom Loader eingelesenen Properties entgegen und
     * initialisiert die notwendigen Paramter
     */
    public RemoteObject(Properties property) {
        // TODO Umstellung auf XML Properties mit automatischer Schemaprüfung
        super();
        System.out.println("=> RemoteObjectProperties()");
        this.props = property;
        this.description = property.getProperty("description");
        this.servicetyp = property.getProperty("typ");
        this.codebase = property.getProperty("codebase");
        this.compClassName = property.getProperty("compClassName");
        this.compName = property.getProperty("compName");
        this.port = property.getProperty("port");
        this.hostname = property.getProperty("hostname");
        this.authTyp = property.getProperty("authTyp");
        this.rmiName = "rmi://" + this.hostname + ":" + this.port + "/";
        System.out.println("<= RemoteObjectProperties() > " + this);
    }

    public String toString() {
        //		TODO auf die aktuellen Attribute anpassen !!!
        StringBuffer sb = new StringBuffer("Service : " + " \thostname: " + this.hostname + "\tcodebase: " + this.codebase +
            " \tdescription: " + this.description + " \tservicetyp: " + this.servicetyp + " \tcompClassName: " +
            this.compClassName + " \tcompName: " + this.compName + " \ttargetPort: " + this.port);
        String s = sb.toString();
        return s;
    }

    /** @return */
    public String getDescription() {
        return description;
    }

    /** @return */
    public String getIpaddr() {
        return ipaddr;
    }

    /** @return */
    public String getServicetyp() {
        return servicetyp;
    }

    /** @param string */
    public void setDescription(String string) {
        description = string;
    }

    /** @param string */
    public void setIpaddr(String string) {
        ipaddr = string;
    }

    /** @param string */
    public void setServicetyp(String string) {
        servicetyp = string;
    }

    /** @return */
    public String getCodebase() {
        return codebase;
    }

    /** @return */
    public String getCompClassName() {
        return compClassName;
    }

    /** @return */
    public String getCompName() {
        return compName;
    }

    /** @return */
    public String getPort() {
        return port;
    }

    /** @param string */
    public void setCodebase(String string) {
        codebase = string;
    }

    /** @param string */
    public void setCompClassName(String string) {
        compClassName = string;
    }

    /** @param string */
    public void setCompName(String string) {
        compName = string;
    }

    /** @param string */
    public void setPort(String string) {
        port = string;
    }

    /** @return */
    public String getHostname() {
        return hostname;
    }

    /** @param string */
    public void setHostname(String string) {
        hostname = string;
    }

    /** @return */
    public String getRmiName() {
        return rmiName;
    }

    /** @param string */
    public void setRmiName(String string) {
        rmiName = string;
    }

    /** @return Application as Client */
    public Application getApp() {
        return app;
    }

    /** @param application */
    public void setApp(Application application) {
        app = application;
    }

    /** @return */
    public Properties getProps() {
        return props;
    }

    /** @param properties */
    public void setProps(Properties properties) {
        props = properties;
    }

    public String getAuthTyp() { return authTyp; }

    public void setAuthTyp(String authTyp) { this.authTyp = authTyp; }
}
