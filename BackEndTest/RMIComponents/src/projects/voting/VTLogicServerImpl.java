package projects.voting;

import java.rmi.RemoteException;
import java.util.Enumeration;

import projects.interfaces.VTClient;
import projects.interfaces.VTLogicServer;
import projects.voting.control.HelperXmlPersitence;
import projects.voting.model.Vote;
import projects.voting.model.VoteTable;
import API.control.Server;
import API.interfaces.ServerHandle;
import API.model.RemoteObject;

/**
 * Implementierung der VTLogicServer Funktionalität.
 * @author danny
 * @since 05.05.2004 20:37:39
 */
public class VTLogicServerImpl extends Server implements VTLogicServer {

    /**
     * die Votes
     */
    private VoteTable votes= null;
    private HelperXmlPersitence hxp;
    //private HelperDatabasePersistence hdp;
    private VTLogicServer logicserver;

    /** 
     * Start des Servers und einlesen der Votes.
     * @throws <{RemoteException}>
     */
    public VTLogicServerImpl() throws RemoteException {
        System.out.println("=> VTLogicServerImpl.VTLogicServerImpl()");
        hxp= new HelperXmlPersitence("shellvotes");
		//hdp= new HelperDatabasePersistence("shellvotes", );
		
        System.out.println("<= VTLogicServerImpl.VTLogicServerImpl()");
    }
	
	/** 
		 * Start des Servers und einlesen der Votes.
		 * @throws <{RemoteException}>
		 */
		public VTLogicServerImpl(ServerHandle manager) throws RemoteException {
			System.out.println("=> VTLogicServerImpl.VTLogicServerImpl()");
			//hxp= new HelperXmlPersitence("shellvotes");
			//hdp= new HelperDatabasePersistence("shellvotes", );
			logicserver = (VTLogicServer) manager;
			
			
			System.out.println("<= VTLogicServerImpl.VTLogicServerImpl()");
		}
    /**
     * Update aller registrierten Clients. Wenn ein Client nicht 
     * erreichbar ist, wird er aus der Liste entfernt.
     */
    public synchronized void updateClients() {
        System.out.println("=> VTLogicServerImpl.updateClients()");
        Enumeration elem= remoteObjects.elements();
        while (elem.hasMoreElements()) {
            RemoteObject remoteObject= null;
            try {
                remoteObject= (RemoteObject) elem.nextElement();
                System.out.println("try update > " + remoteObject + "\n");
                VTClient client= (VTClient) remoteObject.getApp();
                System.out.println("\tfor remoteObject" + client + "\n");
                client.update(votes);
            } catch (RemoteException e) {
                e.printStackTrace();
                System.out.println(
                    "Fehler in VTLogicServerImpl.updateClients() "
                        + "update fehlgeschlagen:\n\t"
                        + remoteObject
                        + " > "
                        + e.getMessage());
                remoteObjects.remove(remoteObject);
            } catch (ClassCastException e) {
                e.printStackTrace();
                System.out.println(
                    "Fehler in VTLogicServerImpl.updateClients() "
                        + "update fehlgeschlagen:\n\t"
                        + remoteObject
                        + " > "
                        + e.getMessage());
                remoteObjects.remove(remoteObject);
            } catch (Exception e) {
                e.printStackTrace();
                System.out.println(
                    "Fehler in VTLogicServerImpl.updateClients() "
                        + "update fehlgeschlagen:\n\t"
                        + remoteObject
                        + " > "
                        + e.getMessage());
                remoteObjects.remove(remoteObject);
            }
        }
        System.out.println("<= VTLogicServerImpl.updateClients()");
    }

    /**
     * Registrierung des Clients. Ruft auf dem Client die update() 
     * Methode auf und übergibt im somit die derzeitigen Daten.
     * @param projects.projects.Client - der registriert werden soll.
     * @throws <{RemoteException}>
    */
    public synchronized String register(RemoteObject remoteObject)
        throws RemoteException {
        System.out.println("=> VTLogicServerImpl.register()");
        String result= super.registerComponent(remoteObject);
        VTClient app= (VTClient) remoteObject.getApp();
        app.update(votes);
        System.out.println("Status " + result + " | client: " + app);
        System.out.println("<= VTLogicServerImpl.register()");
        return result;
    }
    /**
     * ruft in der votetable die save() welche 
     * für persitenz in der ....votes_config.xml sorgt
     *
     */
    public synchronized void save() {
        hxp.save();
    }

    /**
     * Abgabe eines Votes.
     * @param voteid - des gewählten Votes.
     */
    public synchronized void vote(String voteid) throws RemoteException {
        if (votes.containsKey(voteid)) {
            Vote vote= (Vote) votes.get(voteid);
            vote.setCount(vote.getCount() + 1);
            save();
        }
        this.updateClients();
    }

    /* (non-Javadoc)
     * @see projects.interfaces.VTLogicServer#webvote(java.lang.String)
     */
    public VoteTable webvote(String voteid) throws RemoteException {
        if (votes.containsKey(voteid)) {
            Vote vote= (Vote) votes.get(voteid);
            vote.setCount(vote.getCount() + 1);
        }
        this.updateClients();
        return votes;
    }

}
