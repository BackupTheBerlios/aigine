package projects.interfaces;

import projects.catalog.control.CSession;
import API.interfaces.ServerHandle;





/**
 * @author your mama
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public interface CLoginServer extends ServerHandle {
	
	CManagerServer manager = null;
	
	/**
	 * eine Session die ich mir selber zurecht gefummtel habe.
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
	
	CManagerServer getManager()throws java.rmi.RemoteException;
	void setManager(CManagerServer manager)throws java.rmi.RemoteException;
	
}