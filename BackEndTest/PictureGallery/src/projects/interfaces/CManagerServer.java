/*
 * Created on 08.09.2004
 *
 * Created for RMIComponents project :Q
 */
package projects.interfaces;

import java.rmi.RemoteException;

import projects.catalog.control.CSession;

import API.control.WebServer;
import API.interfaces.ServerHandle;
import API.model.RemoteObject;

/**
 * 
 * 
 * class CManagerServer.java created by @author your mama or at his system
 * created on 08.09.2004 2004 at 00:22:14 
 */
public interface CManagerServer extends ServerHandle {
	
	/**
	 * get an registered CDBServer from his RemoteObjectTable
	 * non-overriding method for CManagerServer
	 * Tags are:@return
	 * @return CDBServer
	 */
// TODO kann entfernt werden
	CDBServer getDBServer() throws RemoteException;
	
	/**
	 * get an registered CLoginServer from his RemoteObjectTable
	 * non-overriding method for CManagerServer
	 * Tags are:@return
	 * @return CLoginServer
	 */
//	 TODO kann entfernt werden
	CLoginServer getLoginServer()throws RemoteException;
	
	/**
	 * get an registered WebServer from his RemoteObjectTable
	 * non-overriding method for CManagerServer
	 * Tags are:@return
	 * @return WebServer
	 */
//	 TODO kann entfernt werden
	WebServer getWebServer()throws RemoteException;
	
	/**
	 * set a given CDBServer to his attribute and also add him to the RemoteObjectTable 
	 * non-overriding method for CManagerServer
	 * Tags are:@param dbserver
	 * @return void
	 */
	void setDBServer(CDBServer dbserver)throws RemoteException;
	
	/**
	 * set a given CLoginServer to his attribute and also add him to the RemoteObjectTable
	 * non-overriding method for CManagerServer
	 * Tags are:@param loginserver
	 * @return void
	 */
	void setLoginServer(CLoginServer loginserver)throws RemoteException;
	
	/**
	 * set a given WebServer to his attribute and also add him to the RemoteObjectTable
	 * non-overriding method for CManagerServer
	 * Tags are:@param webserver
	 * @return void
	 */
	void setWebServer(WebServer webserver)throws RemoteException;
	
	/**
	* gibt einen bestimmten Service zurueck, wenn er bereits registriert ist
	* @param String ServiceName
	* @return 
	* @throws RemoteException
	*/
	RemoteObject getService(String ServiceName) throws RemoteException ;
	
	/**
	 * createSession() 
	 * 
	 * 
	 * class CManagerServer.java created by @author your mama or at his system
	 * created on 10.09.2004 2004 at 18:09:18
	 */
	CSession createSession(long usrID)throws RemoteException;
	

}
