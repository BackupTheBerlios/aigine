/*
 * Created on 08.09.2004
 *
 * Created for PictureGallary project :Q
 */
package projects.catalog;

import java.rmi.RemoteException;
import java.rmi.server.RMIClientSocketFactory;
import java.rmi.server.RMIServerSocketFactory;
import java.util.Hashtable;

import API.control.Database;
import projects.catalog.model.Category;
import projects.catalog.model.Picture;
import projects.interfaces.CDBServer;
import API.interfaces.ManagerHandle;
import API.model.RemoteObject;

/**
 * 
 * 
 * class CDBServerImpl.java created by @author ?, drichter
 * created on 08.09.2004 2004 at 14:14:46 
 */
public class CDBServerImpl extends Database implements CDBServer {

	//TODO initialisiert die voting.control.HelperDatabasePersistence
	private String WebRequestError = null;
	private ManagerHandle manager;	
	
	/**
	 * description:
	 * 
	 * @param remoteObject
	 * @param usr
	 * @param pass
	 * @return
	 * @throws RemoteException 
	 * @see API.control.Server#register(API.model.RemoteObject, java.lang.String, java.lang.String)
	 */
	public synchronized String register(RemoteObject remoteObject, String usr,
			String pass) throws RemoteException {
		// LATER gegister methode mit usr and passwd (for adminClients mybe)
		String regresult = super.register(remoteObject, usr, pass); 
		return regresult ;
	}
	/**
	 * description:
	 * 
	 * @param remoteObject
	 * @return 
	 * @see API.control.Database#register(API.model.RemoteObject)
	 */
	public synchronized String register(RemoteObject remoteObject) {
		return super.register(remoteObject);
	}
	/**
	 * description:
	 * 
	 * @param remoteObject
	 * @return 
	 * @see API.control.Server#registerComponent(API.model.RemoteObject)
	 */
	protected synchronized String registerComponent(RemoteObject remoteObject) {
		return super.registerComponent(remoteObject);
	}
	/**
	 * Constructor for CDBServerImpl object
	 * @throws RemoteException
	 * 
	 */
	public CDBServerImpl() throws RemoteException {
		super();
	}
	/**
	 * Constructor for CDBServerImpl object
	 * @param port
	 * @throws RemoteException
	 * 
	 */
	public CDBServerImpl(int port) throws RemoteException {
		super(port);
	}
	/**
	 * Constructor for CDBServerImpl object
	 * @param port
	 * @param csf
	 * @param ssf
	 * @throws RemoteException
	 * 
	 */
	public CDBServerImpl(int port, RMIClientSocketFactory csf,
			RMIServerSocketFactory ssf) throws RemoteException {
		super(port, csf, ssf);
	}
	/**
	 * description:
	 * 
	 * @param remoteObject
	 * @param service
	 * @return
	 * @throws RemoteException 
	 * @see API.interfaces.ServerHandle#registerService(API.model.RemoteObject, API.interfaces.ServerHandle)
	 */
	public String registerService(RemoteObject remoteObject) throws RemoteException {
		// Question notwendig?
		return null;
	}

	/**
	 * description: wird beim Registrieren bei einem Manager verwendet
	 * @param manager 
	 * @see projects.interfaces.CLoginServer#setManager(projects.interfaces.CManagerServer)
	 */
//	public void setManager(ManagerHandle localmanager) {
//		this.manager = localmanager ;
//	}

	/* (non-Javadoc)
	 * @see projects.interfaces.CDBServer#setPicture(java.lang.String, java.lang.String, projects.catalog.model.Picture)
	 */
/*	public void setPicture(String catalogName, String categoryName, Picture pic) {
		// TODO unausgereiftes Fragment für die Bilderdatenbank
		
	}*/
	/* (non-Javadoc)
	 * @see projects.interfaces.CDBServer#getManager()
	 */
	public ManagerHandle getManager() throws RemoteException {
		return this.manager ;
	}
	/**
	 * description:
	 * 
	 * @param catalogName, categoryName, pic
	 * @throws RemoteException 
	 * @see projects.interfaces.CDBServer#insertPicture(java.lang.String, java.lang.String, projects.catalog.model.Picture)
	 */
	public void insertPicture(String catalogName, String categoryName, Picture pic) throws RemoteException {
		// TODO Auto-generated method stub
		
	}
	/**
	 * description:
	 * 
	 * @param pic
	 * @throws RemoteException 
	 * @see projects.interfaces.CDBServer#insertPicture(projects.catalog.model.Picture)
	 */
	public void insertPicture(Picture pic) throws RemoteException {
		// TODO Auto-generated method stub
		
	}
	
	public Picture getPicture(String ID) {
		Picture thepic = null ;
		// TODO nur vorerst Zugriff auf die Hashtable mit den händisch vorgegebenen Fotos !!!
		Hashtable provPics = getPictures("Pferde", 1, 0) ;
		Integer num = new Integer(ID) ;
	//	ID.
		
		if (provPics.containsKey(num)) {
			thepic = (Picture) provPics.get(num) ;
		}
		return thepic ; 
	}
	
	
	/**
	 * Description: liefert eine Hashtable zurück mit einer Liste von Picture-Objekten aus der Datenbank, momentan nach Kategorie unterschieden
	 * 
	 * @param Name der Kategorie, Nummer des ersten gewünschten Bildes, max. Anzahl der Bilder
	 * @throws RemoteException 
	 * @see projects.interfaces.CDBServer#getPicture(projects.catalog.model.Picture)
	 */
	public Hashtable getPictures(String categoryName, int StartNr, int Bilderanzahl) {
		Hashtable ht = new Hashtable() ;
		// TODO hier muss nun ein Zugriff auf die Datenbank erfolgen, und die Antwortzeilen werden dann in Picture-Objekte verpackt
		// da dies bislang noch nicht geht, werden die Picture-Objekte hier nun von Hand erstellt
		System.out.println("==> projects.catalog.CDBServerImpl.getPictures") ;
		
		Picture pic = new Picture("img/pferde/img_1.jpg", "JPEG", "Ardenner (Kaltblut)") ;
		pic.setId(1) ;
		ht.put(new Integer(1), pic) ;
		pic = new Picture("img/pferde/img_2.jpg", "JPEG", "Camargue-Pferd") ;
		pic.setId(2) ;
		ht.put(new Integer(2), pic) ;
		pic = new Picture("img/pferde/img_3.jpg", "JPEG", "Brandenburger") ;
		pic.setId(3) ;
		ht.put(new Integer(3), pic) ;
		pic = new Picture("img/pferde/img_4.jpg", "JPEG", "Araber-Mix") ;
		pic.setId(4) ;
		ht.put(new Integer(4), pic) ;
		pic = new Picture("img/pferde/img_5.jpg", "JPEG", "Dartmoor-Pony") ;
		pic.setId(5) ;
		ht.put(new Integer(5), pic) ;
		pic = new Picture("img/pferde/img_6.jpg", "JPEG", "Shetland-Pony") ;
		pic.setId(5) ;
		ht.put(new Integer(6), pic) ;
		System.out.println("<== projects.catalog.CDBServerImpl.getPictures") ;
		return ht ;
	}
	
	/**
	 * description:
	 * 
	 * @param bilderanzahl
	 * @return
	 * @throws RemoteException 
	 * @see projects.interfaces.CDBServer#selectCategory(int)
	 */
/*	public Category selectCategory(int bilderanzahl) throws RemoteException {
		// TODO Auto-generated method stub
		return null;
	} */
}
