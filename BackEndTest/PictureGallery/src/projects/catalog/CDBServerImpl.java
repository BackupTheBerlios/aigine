/*
 * Created on 08.09.2004
 *
 * Created for PictureGallary project :Q
 */
package projects.catalog;

import java.rmi.RemoteException;
import java.rmi.server.RMIClientSocketFactory;
import java.rmi.server.RMIServerSocketFactory;
import java.util.ArrayList;
import java.util.Hashtable;

import API.control.DbServer;
import projects.catalog.db.*;
import projects.catalog.model.Category;
import projects.catalog.model.PictureDTO;
import projects.interfaces.CDbServerHandle;
import API.interfaces.DbServerHandle;
import API.interfaces.ManagerHandle;
import API.model.AbstractDAO;
import API.model.RemoteObject;

/**
 * 
 * 
 * class CDBServerImpl.java created by @author ?, drichter
 * created on 08.09.2004 2004 at 14:14:46 
 */
public class CDBServerImpl extends DbServer implements CDbServerHandle {
//	public final int PICTURE = 0 ;
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

	/* (non-Javadoc)
	 * @see projects.interfaces.CDBServer#getManager()
	 */
	public ManagerHandle getManager() throws RemoteException {
		return this.manager ;
	}
	
	/* (non-Javadoc)
	 * @see API.control.DbServer#initDAOList()
	 */
	protected AbstractDAO[] initDAOList() {
		ArrayList theList = new ArrayList() ;
		PictureDAO theDao = new PictureDAO() ;
		theList.add(theDao) ; // PICTURE = 0 
		return (AbstractDAO[]) theList.toArray(new AbstractDAO[theList.size()]) ;
	}
	
	/* (non-Javadoc)
	 * @see API.control.DbServer#initConnectionValues()
	 */
	protected void initConnectionValues() {
		this.setDbname("gallerydb") ;
		this.setDbuser("postgres") ;
		this.setDbpasswd("postgres") ;
	}


/*	
	public PictureDTO getPicture(String ID) {
		PictureDTO thepic = null ;
		// TODO nur vorerst Zugriff auf die Hashtable mit den händisch vorgegebenen Fotos !!!
		Hashtable provPics = getPictures("Pferde", 1, 0) ;
		Integer num = new Integer(ID) ;
	//	ID.
		
		if (provPics.containsKey(num)) {
			thepic = (PictureDTO) provPics.get(num) ;
		}
		return thepic ; 
	}
	*/
	
	/**
	 * Description: liefert eine Hashtable zurück mit einer Liste von Picture-Objekten aus der Datenbank, momentan nach Kategorie unterschieden
	 * 
	 * @param Name der Kategorie, Nummer des ersten gewünschten Bildes, max. Anzahl der Bilder
	 * @throws RemoteException 
	 * @see projects.interfaces.CDBServer#getPicture(projects.catalog.model.Picture)
	 */
/*
	public Hashtable getPictures(String categoryName, int StartNr, int Bilderanzahl) {
		Hashtable ht = new Hashtable() ;
		// TODO hier muss nun ein Zugriff auf die Datenbank erfolgen, und die Antwortzeilen werden dann in Picture-Objekte verpackt
		// da dies bislang noch nicht geht, werden die Picture-Objekte hier nun von Hand erstellt
		System.out.println("==> projects.catalog.CDBServerImpl.getPictures") ;
		
		PictureDTO pic = new PictureDTO("img/pferde/img_1.jpg", "JPEG", "Ardenner (Kaltblut)") ;
		pic.setId(1) ;
		ht.put(new Integer(1), pic) ;
		pic = new PictureDTO("img/pferde/img_2.jpg", "JPEG", "Camargue-Pferd") ;
		pic.setId(2) ;
		ht.put(new Integer(2), pic) ;
		pic = new PictureDTO("img/pferde/img_3.jpg", "JPEG", "Brandenburger") ;
		pic.setId(3) ;
		ht.put(new Integer(3), pic) ;
		pic = new PictureDTO("img/pferde/img_4.jpg", "JPEG", "Araber-Mix") ;
		pic.setId(4) ;
		ht.put(new Integer(4), pic) ;
		pic = new PictureDTO("img/pferde/img_5.jpg", "JPEG", "Dartmoor-Pony") ;
		pic.setId(5) ;
		ht.put(new Integer(5), pic) ;
		pic = new PictureDTO("img/pferde/img_6.jpg", "JPEG", "Shetland-Pony") ;
		pic.setId(5) ;
		ht.put(new Integer(6), pic) ;
		System.out.println("<== projects.catalog.CDBServerImpl.getPictures") ;
		return ht ;
	}
	*/
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
