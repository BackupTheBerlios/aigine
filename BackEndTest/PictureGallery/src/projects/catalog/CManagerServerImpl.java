/*
 * Created on 08.09.2004
 *
 * Created for RMIComponents project :Q
 */
package projects.catalog;

import java.rmi.RemoteException;

import projects.catalog.control.CSession;
import projects.interfaces.CAdminClient;
import projects.interfaces.CClient;
import projects.interfaces.CDBServer;
import projects.interfaces.CLoginServer;
import projects.interfaces.CManagerServer;
import projects.interfaces.CProjectServer;

import API.control.Manager;
import API.control.WebServer;

import API.model.RemoteObject;
import API.model.RemoteObjectTable;

/**
 * 
 * 
 * class CManagerServerImpl.java created by @author your mama or at his system
 * created on 08.09.2004 2004 at 00:20:21 
 */
public class CManagerServerImpl extends Manager implements CManagerServer {

	//RemoteObject
	
	/**
	 * Constructor for CManagerServerImpl object
	 * @throws RemoteException
	 * 
	 */
	public CManagerServerImpl() throws RemoteException {
		super();
		// TODO Auto-generated constructor stub
	}

	/**
	 * description:
	 * 
	 * @param service
	 * @return
	 * @throws RemoteException 
	 * @see API.interfaces.ServerHandle#register(API.model.RemoteObject)
	 */
	public String register(RemoteObject client) throws RemoteException {
		// TODO Auto-generated method stub
		super.registerComponent(client) ;
		return null;
	}

	/**
	 * description:
	 * 
	 * @param remoteObject
	 * @param service
	 * @return
	 * @throws RemoteException 
	 * @see API.interfaces.ServerHandle#registerService(API.model.RemoteObject)
	 */
		public synchronized String registerService(final RemoteObject remoteObject)
		throws RemoteException {
			// TODO kann vereinfacht und vereinheitlicht werden, anhand des Typs unterscheiden ob getServerApp oder getApp
			// dürfte in der momentanen Variante auch nicht funktionieren wegen der String-Vergleiche
			//String result = new String("") ;
			
	        System.out.println("=> CManagerServerImpl.registerService()remoteObj= "+remoteObject);	             
	        String result = super.registerService(remoteObject);
	        System.out.println(remoteObject.toString()) ;
	        if(remoteObject.getCompClassName() == "CDBServer") {
	        	CDBServer db = (CDBServer) remoteObject.getServerApp();
	        	db.setManager(this);
	        }
	        if(remoteObject.getCompClassName() == "CLoginServer") {
	        	CLoginServer login = (CLoginServer) remoteObject.getServerApp();
	        	login.setManager(this);
	        }
	        if(remoteObject.getCompClassName() == "WebServer") {
	        	CClient web = (CClient) remoteObject.getApp();
	        	web.setManager(this);
	        }
	        if(remoteObject.getCompClassName() == "CAdminClient") {
	        	CAdminClient admclient = (CAdminClient) remoteObject.getServerApp();
	        	admclient.setManager(this);
	        }
	        if(remoteObject.getCompClassName() == "CClient") {
	        	CClient client = (CClient) remoteObject.getApp();
	        	client.setManager(this);
	        }
			if(remoteObject.getCompClassName().indexOf("CProjectServer") == 0) {
				System.out.println("versuche bei einem CProjectServer .setManager anzuwenden ...\n\n") ;
				CProjectServer cproject = (CProjectServer) remoteObject.getServerApp();
				cproject.setManager(this) ;
			} 
	        
	        System.out.println("CManagerServerImpl.registerService() > neues RemoteObject registriert.");
	        System.out.println("<= CManagerServerImpl.registerService()");
		return result;
	}

	/**
	 * description:
	 * 
	 * @param String ServiceName
	 * @return den geangeforderten Server, wenn nicht registriert <code>null</code>
	 * @throws RemoteException 
	 * @see API.interfaces.ServerHandle#getService(API.model.RemoteObject)
	 */
	public synchronized RemoteObject getService(String ServiceName) throws RemoteException {
		System.out.println("=> CManagerServerImpl.getService()ServiceName = " + ServiceName) ;
		RemoteObject ro = super.getService(ServiceName);
		System.out.println("<= CManagerServerImpl.getService()") ;
		return ro ;
	}

	/**
	 * description:
	 * 
	 * @param remoteObject
	 * @param usr
	 * @param pass
	 * @return
	 * @throws RemoteException 
	 * @see API.interfaces.ServerHandle#register(API.model.RemoteObject, java.lang.String, java.lang.String)
	 */
	public String register(RemoteObject remoteObject, String usr, String pass) throws RemoteException {
		// TODO Auto-generated method stub
		return super.register(remoteObject, usr, pass) ;
	}

	/**
	 * description:
	 * 
	 * @param remoteObject
	 * @param usr
	 * @param pass
	 * @return
	 * @throws RemoteException 
	 * @see API.interfaces.ServerHandle#registerAdminClient(API.model.RemoteObject, java.lang.String, java.lang.String)
	 */
	public String registerAdminClient(RemoteObject remoteObject, String usr, String pass) throws RemoteException {
		// TODO Auto-generated method stub
		return null;
	}

	/**
	 * description:
	 * 
	 * @return
	 * @throws RemoteException 
	 * @see API.interfaces.ServerHandle#checkClientConnections()
	 */
	public RemoteObjectTable checkClientConnections() throws RemoteException {
		// TODO Auto-generated method stub
		return null;
	}

	/**
	 * description:
	 * 
	 * @return 
	 * @see projects.interfaces.CManagerServer#getDBServer()
	 */
	public CDBServer getDBServer()throws RemoteException{
		
		
	
		return null;
	}

	/**
	 * description:
	 * 
	 * @return 
	 * @see projects.interfaces.CManagerServer#getLoginServer()
	 */
	public CLoginServer getLoginServer() throws RemoteException{
		// TODO Auto-generated method stub
		return null;
	}

	/**
	 * description:
	 * 
	 * @return 
	 * @see projects.interfaces.CManagerServer#getWebServer()
	 */
	public WebServer getWebServer() throws RemoteException{
		// TODO Auto-generated method stub
		return null;
	}

	/**
	 * description:
	 * 
	 * @param dbserver 
	 * @see projects.interfaces.CManagerServer#setDBServer(projects.interfaces.CDBServer)
	 */
	public void setDBServer(CDBServer dbserver) throws RemoteException{
		// TODO Auto-generated method stub
		System.out.println("hallo");
	}

	/**
	 * description:
	 * 
	 * @param loginserver 
	 * @see projects.interfaces.CManagerServer#setLoginServer(projects.interfaces.CLoginServer)
	 */
	public void setLoginServer(CLoginServer loginserver) throws RemoteException{
		// TODO Auto-generated method stub
		
	}

	/**
	 * description:
	 * 
	 * @param webserver 
	 * @see projects.interfaces.CManagerServer#setWebServer(API.control.WebServer)
	 */
	public void setWebServer(WebServer webserver) throws RemoteException{
		// TODO Auto-generated method stub
		
	}

	
	
	/**
	 * description:
	 * 
	 * @param usrID
	 * @return
	 * @throws RemoteException 
	 * @see projects.interfaces.CManagerServer#createSession(long)
	 */
	public CSession createSession(long usrID) throws RemoteException {
		RemoteObject ro = this.getService("CLoginServer") ;
		if (ro == null) {
			return null ;
		}
		CLoginServer cloginserver = (CLoginServer) ro.getServerApp() ;
		cloginserver.createSession(usrID) ;
		// TODO SessionID ?!
		return null;
	}

}
