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
	
	public Hashtable getPictures(String categoryName, int StartNr) {
		Hashtable ht = new Hashtable() ;
		// TODO hier muss nun ein Zugriff auf die Datenbank erfolgen, und die Antwortzeilen werden dann in Picture-Objekte verpackt
		// da dies bislang noch nicht geht, werden die Picture-Objekte hier nun von Hand erstellt
		Picture pic = new Picture("img/pferde/img_1.jpg", "JPEG", "Schönes Pferd 1") ;
		ht.put(new Integer(0), pic) ;
		pic = new Picture("img/pferde/img_2.jpg", "JPEG", "Hübsches Pferd 2") ;
		ht.put(new Integer(1), pic) ;
		pic = new Picture("img/pferde/img_3.jpg", "JPEG", "Tolles Pferd 3") ;
		ht.put(new Integer(2), pic) ;
		pic = new Picture("img/pferde/img_4.jpg", "JPEG", "Super Pferd 4") ;
		ht.put(new Integer(3), pic) ;
		pic = new Picture("img/pferde/img_5.jpg", "JPEG", "Ruhiges Pferd 5") ;
		ht.put(new Integer(4), pic) ;
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
