/*
 * Created on 14.03.2005
 *
 * @user drichter
 * */
package API.portal;

import API.control.DbServer ;
import API.interfaces.DbServerHandle ;
// import API.interfaces.ServerHandle;
import API.model.AbstractDAO;
import API.portal.model.RequestFrameSet;


import java.rmi.RemoteException;

/**
 * 
 * @author drichter
 * 
 */
public class PortalDbServerImpl extends DbServer implements DbServerHandle {
	private RequestFrameSet frameSet = null ; // DR: Diese Variable dient nur den Platzhaltertests ohne echte Datenbanknutzung
	

	/**
	 * Constructor :
	 * @throws RemoteException
	 *  
	 */
	public PortalDbServerImpl() throws RemoteException {
		super();
	}
	
	/* (non-Javadoc)
	 * @see API.control.DbServer#initDAOList()
	 */
	protected AbstractDAO[] initDAOList() {
		// TODO muss mit den PortalDB-DAOs konfiguriert werden
		return null ;
	}

	/* (non-Javadoc)
	 * @see API.control.DbServer#initConnectionValues()
	 */
	protected void initConnectionValues() {
		this.setDbname("portaldb") ;
		this.setDbuser("postgres") ;
		this.setDbpasswd("postgres") ;
	}

}

