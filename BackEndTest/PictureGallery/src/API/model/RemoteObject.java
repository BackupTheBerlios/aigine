package API.model;

import java.io.Serializable;

import API.control.Server;
import API.interfaces.Application;
import API.interfaces.ServerHandle;
/**
 * Speichert die Eigenschaften einer RMI Komponente (Verbindungsinformationen etc.)
 * @author danny, tobi, franky
 * @since 25.07.2004
 * @version 0.01
 */
public class RemoteObject implements Serializable {
	private String username;

	private String passwd;

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

	/** Name der Klasse für die zu ladene Komponente, z.B. projects.voting.VTLogicServer */
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
	
	/**
	 * Referenz auf die Instanz des ServerHandlers, wenn es sich um einen solchen handelt;
	 */
	protected ServerHandle serverApp;

	/** (byDR)
	 * Referenz auf die Instanz der Server-Klasse, wenn es sich um eine solche handelt;
	 */
	// TODO unsinnig???
	protected Server theServer;

	/** Authentifizierungstyp, derzitig anonym, nothing, password */
	protected String authTyp;

	/**
	 * zusaetzliche property für den service RMI name
	 */
	protected String manager;

	/**
	 * name des Managers an dem sich registriert wird
	 *
	 */
	protected String managerName;

	/**
	 * Konstruktor nimmt die vom Loader eingelesenen Properties entgegen und
	 * initialisiert die notwendigen Paramter
	 */
	public RemoteObject() {

		super();
		//configuration.getProperty("codebase","nix","vtserver");
	}

	public String toString() {
		//		TODO auf die aktuellen Attribute anpassen !!!
		StringBuffer sb =
			new StringBuffer(
				"Service : "
					+ " \thostname: "
					+ this.hostname
					+ "\tcodebase: "
					+ this.codebase
					+ " \tdescription: "
					+ this.description
					+ " \tservicetyp: "
					+ this.servicetyp
					+ " \tcompClassName: "
					+ this.compClassName
					+ " \tcompName: "
					+ this.compName
					+ " \ttargetPort: "
					+ this.port
					+ "\tmanager: "
					+ this.manager
					+ "\tmanagerName: "
					+ this.managerName
					+ "\tServerApp: "
					+ this.serverApp);
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

	/** @return client oder server 
	 *  */
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
		this.serverApp = null;
	}

	public String getAuthTyp() {
		return authTyp;
	}

	public void setAuthTyp(String authTyp) {
		this.authTyp = authTyp;
	}
	/**
	 * @return
	 */
	public String getPasswd() {
		return passwd;
	}

	/**
	 * @param i
	 */
	public void setPasswd(String i) {
		passwd = i;
	}

	/**
	 * @return
	 */
	public String getUsername() {
		return username;
	}

	/**
	 * @param i
	 */
	public void setUsername(String i) {
		username = i;
	}

	/**
	 * @return
	 */
	public String getManager() {
		return manager;
	}

	/**
	 * @param string
	 */
	public void setManager(String string) {
		manager = string;
	}

	/**
	 * @return
	 */
	public String getManagerName() {
		return managerName;
	}

	/**
	 * @param string
	 */
	public void setManagerName(String string) {
		managerName = string;
	}

	/**
	 * @return Returns the serverApp.
	 */
	public ServerHandle getServerApp() {
		return serverApp;
	}
	
	/**
	 * @param serverApp The serverApp to set.
	 */
	public void setServerApp(ServerHandle serverApp) {
		this.serverApp = serverApp;
		this.app = null;
	}
	
	/**
	 * @return Returns the serverApp.
	 */
	// TODO unsinnig???
	public Server gettheServer() {
		return this.theServer;
	}
	
	/**
	 * @param server. The Server to set.
	 */
	// TODO unsinnig???
	public void settheServer(Server localServer) {
		this.theServer = localServer;
	}
}
