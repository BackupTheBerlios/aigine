/*
 * Created on 08.09.2004
 *
 * Created for PictureGallary project :Q
 */
package projects.interfaces;

import java.rmi.RemoteException;

import API.interfaces.Client;
import API.interfaces.ManagerHandle ;

/**
 * 
 * 
 * class CAdminClient.java created by @author your mama or at his system
 * created on 08.09.2004 2004 at 09:53:02 
 */
public interface CAdminClient extends Client {
	
	ManagerHandle manager = null;
	
	
	void setManager(ManagerHandle manager) throws RemoteException;
	
	ManagerHandle getManager() throws RemoteException;

}
