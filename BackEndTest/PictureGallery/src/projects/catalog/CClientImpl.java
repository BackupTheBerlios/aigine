package projects.catalog;

import java.rmi.RemoteException;

import projects.interfaces.CClient;
import projects.interfaces.CManagerServer;
import API.interfaces.ServerHandle;
import API.model.RemoteObject;

/**
 * 
 * @author danny
 * @since 05.05.2004 20:16:24
 */
public class CClientImpl implements CClient {

	

	public void init(RemoteObject compProps, ServerHandle server) {
		System.out.println("=> CClientImpl.init(" +			"RemoteObjectProperties " + compProps + " , Object" + server);
		CManagerServer s = (CManagerServer) server;

		
		System.out.println("<= CClientImpl.init(" +
			"RemoteObjectProperties " + compProps + " , Object" + s);
	}

	
	/**
	 * Initialisiert den ActionListener mit dem Server Objekt, so dass
	 * dieser auf die vote Methode des Servers zugreifen kann.
	 * @param server
	 */
	public void setServerGrap(CManagerServer server) {
		//bv.addActionListener(new VTClientVoteAction(server, vtmodel));
		System.out.println(
			"\t Serverhandle an den Actionlistener übergeben = " + server);
	}


	/**
	 * description:
	 * 
	 * @param manager
	 * @throws RemoteException 
	 * @see projects.interfaces.CClient#setManager(projects.interfaces.CManagerServer)
	 */
	public void setManager(CManagerServer manager) throws RemoteException {
		// TODO Auto-generated method stub
		System.out.println(
				"\t Serverhandle "+manager);
	}


	/**
	 * description:
	 * 
	 * @return
	 * @throws RemoteException 
	 * @see projects.interfaces.CClient#getManager()
	 */
	public CManagerServer getManager() throws RemoteException {
		// TODO Auto-generated method stub
		return null;
	}
}
