package API.control;

import java.rmi.RemoteException;
import java.rmi.server.RMIClientSocketFactory;
import java.rmi.server.RMIServerSocketFactory;
import java.rmi.server.UnicastRemoteObject;
import java.util.Hashtable;

import API.interfaces.AdminClient;
import API.interfaces.ManagerHandle;
import API.interfaces.ServerHandle;
import API.model.RemoteObject;
import API.model.RemoteObjectTable;
import API.portal.model.Block;

/**
 * Implementierung häufig benötiger Serverfunktionalitäten, z.B. Anmeldung von Clients.
 * @author danny, tobi
 * @since 25.07.2004
 * @version 0.01
 */

public abstract class Server extends UnicastRemoteObject {
	private String WebRequestError = null;
	protected ManagerHandle manager = null ;
	
	/**
	 * Die registrierten Kommunikationspartner
	 * entweder Clients oder Services oder wiederum Server ... WHAT EVER !
	 */
	protected RemoteObjectTable remoteObjects = null ;
	
	/**
	 * Wird von den Services für gegenseitige Registrierung benötigt.
	 * @param compProps
	 * @param server
	 */
	public void init(RemoteObject compProps, ServerHandle server) {
		// TODO (?) in extra Klasse Service auslagern, die von Server abgeleitet wird
	}

	/**
	 * @throws java.rmi.RemoteException
	 */
	public Server() throws RemoteException {
		super();
	}

	/**
	 * @param port
	 * @throws java.rmi.RemoteException
	 */
	public Server(int port) throws RemoteException {
		super(port);
	}

	/**
	 * @param port
	 * @param csf
	 * @param ssf
	 * @throws java.rmi.RemoteException
	 */
	public Server(
		int port,
		RMIClientSocketFactory csf,
		RMIServerSocketFactory ssf)
		throws RemoteException {
		super(port, csf, ssf);
	}

	/**
	 * Eintragen des Managers, an welchem der Server angemeldet wird 
	 * @param theManager
	 * @throws RemoteException
	 */
	final public void setManager(ManagerHandle theManager) throws RemoteException {
		this.manager = theManager ;
	}

	/**
	 * Erfragen eines ServerHandle beim Manager, Kapselung der Fehlerprüfungen 
	 * @param theManager
	 * @throws RemoteException
	 */
	protected ServerHandle getServer(String Servername) {
		ServerHandle theServer = null ;
		RemoteObject ro = null ;
		System.out.println("==> API.control.Server.getServer()") ;
		try {
			ro = manager.getService(Servername) ;
		} catch (RemoteException re) {
			System.out.println("--- RemoteException beim Holen eines Servers '" + Servername + "' aufgetreten: " + re) ;
		}
		if (ro != null) {
//			System.out.println("  > Vergleich von '" + ro.getServicetyp() + "' mit 'server' mittels compareTo ergibt " + ro.getServicetyp().compareTo(new String("server")) ) ;
			if (ro.getServicetyp().compareTo(new String("server")) == 0) {
				System.out.println("+++ Ich habe einen Server erhalten!!") ;
				theServer = (ServerHandle) ro.getServerApp() ;
				if (theServer != null) {
					System.out.println("<== API.control.Server.getServer()") ;
					return theServer ;
				} else {
					// Fehler behandeln
					System.out.println("--- angeforderter Server '" + Servername + "' ist bei meinem Manager nicht erreichbar!!!\n") ;
				}
			} else {
				// Fehler behandeln
				System.out.println("--- Übergebenes RemoteObject ist nicht vom benötigten Typ 'server'!\n") ;
			}
		} else {
			// Fehler behandeln
			System.out.println("--- Übergebenes RemoteObject ist null!\n") ;
		}
		System.out.println("<== API.control.Server.getServer()") ;
		return null ;
	}

	/**
	 * Initialisiert die Tabelle mit den Kommunikationspartnern.
	 */
	public void initObjectTable() {
		System.out.println("=> Server.initObjectTable()");
		remoteObjects = new RemoteObjectTable();
		System.out.println("<= Server.initObjectTable()");
	}

	/**
	 * Registrierung eines RemoteObjects, wird von allen anderen register() Methoden
	 * verwendet, um eine mögliche Überblendung zu vermeiden. 
	 * @param service
	 * @throws RemoteException
	 */
	protected synchronized String registerComponent(final RemoteObject remoteObject){
		System.out.println("=> Server.registerComponent(RemoteObject " + remoteObject + ")");
		String status = null;
		if (remoteObjects.contains(remoteObject)) {
			status = " exists";
		} else {
			remoteObjects.add(remoteObject);
			status = " now exists";
		}
		System.out.println("<=Server.registerComponent(RemoteObject " + remoteObject + ")");
		return status;
	}
	
	/**
	 * Registrierung eines RemoteObjects für anonyme Kommunikation.
	 * @param service
	 * @throws RemoteException

	public synchronized String register(final RemoteObject remoteObject)
		throws RemoteException {
		System.out.println("=> Server.register(RemoteObject " + remoteObject + ")");
		System.out.println("TROTTEL, die Methode mußt du doch überschreiben.");
		System.out.println("<= Server.register(RemoteObject " + remoteObject + ")");
		return "überschreiben oder registerComponent nutzen.";
	}
	*/


	/**
	 * Registrierung eines Logik Service
	 * verwendet, um eine mögliche Überblendung zu vermeiden. 
	 * @param service
	 * @throws RemoteException
	 * @throws RemoteException
	 */
	protected synchronized String registerService(final RemoteObject remoteObject) throws RemoteException{
		System.out.println("=> Server.registerComponent(RemoteObject " + remoteObject + ")");
		String status = null;
		if (remoteObjects.contains(remoteObject)) {
			status = " exists";
		} else {
			remoteObjects.add(remoteObject, remoteObject.getManagerName());
			status = " now exists";
		}
		System.out.println("<=Server.registerComponent(RemoteObject " + remoteObject + ")");
		return status;
	}

	/**
		 * Liefert einen registrierten Service zurück, wenn der Klassenname in der
		 * RemoteObjectTable bereits vorhanden ist.
		 * @author dennis
		 * @param serviceName
		 * @throws RemoteException
		 * @return RemoteObject oder null, wenn kein Eintrag gefunden wurde.
		 */
	protected synchronized RemoteObject getService(String ServiceName) throws RemoteException {
		System.out.println("versuche Service " + ServiceName + " zurueck zu liefern ...") ;
		RemoteObject ro = remoteObjects.getService(ServiceName) ; 
		return ro ; 
	}
	
	
	/**
	 * Registrierung eines RemoteObjects mit Username und Password.
	 * @param remoteObject
	 * @param username
	 * @param password
	 * @throws RemoteException
	 */
	public synchronized String register(
		final RemoteObject remoteObject,
		final String usr,
		final String pass)
		throws RemoteException {
		System.out.println(
			"=> Server.register(remotObject, usrname ="
				+ usr
				+ " , password = "
				+ pass
				+ ")");
		String status = null;

		// TODO Testdaten durch Zugriff auf Datenspeicher ersetzem (Usertabelle in DB)!
		if (usr.equals("nice") && pass.equals("yourMama")) {
			status = "tschesch kollega :D  => " + this.registerComponent(remoteObject);
		} else {
			status = "USERNAME oder PASSWORD falsch ! dat war wohl nix :D";
		}

		System.out.println("<= Server.register(mit Password) > " + status);
		return status;
	}
	
	/**
	 * Registrierung eines Remote Admin Objects mit Username und Password.
	 * Wird vom Loader aufgerufen, wenn <code>typ=admin</code>.
	 * @param remoteObject
	 * @param username
	 * @param password
	 * @throws RemoteException
	 */
	public synchronized String registerAdminClient(
		final RemoteObject remoteObject,
		final String usr,
		final String pass)
		throws RemoteException {
		System.out.println(
			"=> Server.registerAdminClient(remotObject, usrname ="
				+ usr
				+ " , password = "
				+ pass
				+ ")");
			
			String status = register(remoteObject, usr, pass);
			System.out.println("\t registriert HOFFENTLICH :D > status:"
				 + status);
			// Daten an Client schicken
			AdminClient app = (AdminClient)remoteObject.getApp();
			System.out.println("\t try update client call > " + app);
			app.update(remoteObjects);
			System.out.println("\t updated client call > " + app);
		return status;
	}	
	
	/**
	* verarbeitet einen 'WebRequest' der momentan vom Webserver an beliebige andere Server geschickt werden kann
	* muss von den Servern dann individuell überschrieben werden, dabei könnten
	* Standardinfos auch hier schon für die Ausgabe implementiert werden.
	* @author Dennis
	* @since 11.09.2004
	* @param Operation, welcher Block, Parameterpaare und evtl. später mit übertragenem Body
	* @return Block (serializable)
	* @throws RemoteException
	*/
	public Block executeWebRequest(String op, String whichBlock, Hashtable requestProps) throws RemoteException {
		Block result = null ;
		WebRequestError = null ;
		System.out.println("\n\n>>> habe einen WebRequest erhalten: " + op) ;
		if (op.indexOf("serverinfo") == 0) {
			result = new Block(this.toString()) ;
			result.setTitle("Serverinformation") ;
			//result.addContent() ;
		} else
		{
			WebRequestError = new String("unexpected Operation") ;
			result = new Block("unexpected Block-Operation") ;
		}
		System.out.println("\n\n  > ich antworte mit: " + result) ;
		return result ;
	}

	/**
	* liefert Fehlerdetails, wenn bei executeWebRequest ein Fehler auftritt
	* wird wahrscheinlich wieder entfallen, da executeWebRequest selbst FehlerBlocks generieren kann.
	* @author Dennis
	* @since 11.09.2004
	* @param keine
	* @return String
	* @throws RemoteException
	*/
	public String getWebRequestError() throws RemoteException {
		return WebRequestError ;
	}
	
	/**
	 * Gibt die aktuelle Tabelle der registrierten Komponenten zurück.
	 * 
	 * @author danny
	 * @since 10.08.2004 02:15:12
	 * @version 0.01
	 * @see API.interfaces.ServerHandle#checkClientConnections()
	 */
	// TODO alle eingetragenden RemoteObjects brauchen eine test() Funktion,
	// so dass der Server einen Check auf allen Objekten ausführen und diese
	// ggf. aus der Tabelle entfernen kann.
	// TODO update() Methode, die alle Clients abklappert und die übergebene Funktionalität
	// auf den Clients ausführt.
	public RemoteObjectTable checkClientConnections() throws RemoteException {
		return remoteObjects;
	}	
}
