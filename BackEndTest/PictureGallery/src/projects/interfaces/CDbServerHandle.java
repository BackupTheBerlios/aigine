/*
 * Created on 10.09.2004
 *
 * Created for PictureGallery project :Q
 */
package projects.interfaces;

// import java.rmi.RemoteException;
// import java.util.Hashtable;

// import projects.catalog.model.Category;
// import projects.catalog.model.PictureDTO;
import API.interfaces.DbServerHandle;
// import API.interfaces.ServerHandle;
// import API.interfaces.ManagerHandle ;

/**
 * CDBServer stellt die einzelnen Objecte zur Verfügung, die in die datenbank gehen
 * und von dort kommen
 * 
 * class CDBServer.java created by @author your mama or at his system
 * created on 10.09.2004 2004 at 08:07:56 
 */
public interface CDbServerHandle extends DbServerHandle {
	final static int PICTURE = 0 ;
	
	
	
	
	// TODO by DR ******************* was hiernach kommt kann wahrscheinlich komplett weg ... ***************************/
	 //setzt der Manager in seiner register methode
/*	ManagerHandle manager = null;
	
	 void insertPicture(String catalogName, String categoryName, PictureDTO pic) throws RemoteException;
	 void insertPicture(PictureDTO pic) throws RemoteException;
	 
	public Hashtable getPictures(String categoryName, int StartNr, int Bilderanzahl) throws RemoteException ;
	public PictureDTO getPicture(String ID) throws RemoteException ;
	 
//	 Category selectCategory(int bilderanzahl) throws RemoteException; 
//	Question (byDR) wie war die obige Methode gedacht???	 
	 
	/**
	  * CManagerServer.registerServices() ruft dann
	  * non-overriding method for CDBServer
	  * Tags are:@param manager
	  * @return void
	  */
/*	 void setManager(ManagerHandle manager)throws RemoteException;
	 ManagerHandle getManager()throws RemoteException; */
}
