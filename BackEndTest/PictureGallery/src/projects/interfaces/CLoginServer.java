package projects.interfaces;

import projects.catalog.control.CSession;
import API.interfaces.ServerHandle ;
import API.interfaces.ManagerHandle ;


/**
 * @author Danny, Tobi?!
 *
 */
public interface CLoginServer extends ServerHandle {
	
	ManagerHandle manager = null;
	
	/**
	 * eine Session die ich mir selber zurecht gefummelt habe.
	 * User meldet sich an, wird in der db gesucht, angelegt/zurückgegeben und 
	 * in diese CSession gespeichert.
	 *  
	 * non-overriding method for CLoginServer
	 * Tags are:@param usrID
	 * Tags are:@return
	 * Tags are:@throws java.rmi.RemoteException
	 * @return CSession
	 */
	CSession createSession(long usrID) throws java.rmi.RemoteException;
	
	ManagerHandle getManager()throws java.rmi.RemoteException;
	void setManager(ManagerHandle manager)throws java.rmi.RemoteException;
	
}