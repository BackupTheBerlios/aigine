/*
 * Created on 10.09.2004
 *
 * Created for PictureGallery project :Q
 */
package projects.interfaces;

import java.rmi.RemoteException;

import projects.catalog.model.Category;
import projects.catalog.model.Picture;
import API.interfaces.ServerHandle;
import API.interfaces.ManagerHandle ;

/**
 * CDBServer stellt die einzelnen Objecte zur Verfügung, die in die datenbank gehen
 * und von dort kommen
 * 
 * class CDBServer.java created by @author your mama or at his system
 * created on 10.09.2004 2004 at 08:07:56 
 */
public interface CDBServer extends ServerHandle {
	 //setzt der Manager in seiner register methode
	ManagerHandle manager = null;
	
	 void insertPicture(String catalogName, String categoryName, Picture pic) throws RemoteException;
	 void insertPicture(Picture pic) throws RemoteException;
	 
	 Category selectCategory(int bilderanzahl) throws RemoteException; 
	 
	 
	 
	 
	 
	 /**
	  * CManagerServer.registerServices() ruft dann
	  * non-overriding method for CDBServer
	  * Tags are:@param manager
	  * @return void
	  */
	 void setManager(ManagerHandle manager)throws RemoteException;
	 ManagerHandle getManager()throws RemoteException;
}
