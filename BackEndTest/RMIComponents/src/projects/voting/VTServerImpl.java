package projects.voting;

import java.io.IOException;
import java.rmi.RemoteException;
import java.util.Enumeration;

import projects.interfaces.VTClient;
import projects.interfaces.VTServer;
import projects.voting.model.Vote;
import projects.voting.model.VoteTable;
import API.control.Server;
import API.model.RemoteObject;

/**
 * Implementierung der VTServer Funktionalität.
 * @author danny
 * @since 05.05.2004 20:37:39
 */
public class VTServerImpl extends Server implements VTServer {

    /**
     * die Votes
     */
    private VoteTable votes= null;

    /** 
     * Start des Servers und einlesen der Votes.
     * @throws <{RemoteException}>
     */
    public VTServerImpl() throws RemoteException {
        System.out.println("=> VTServerImpl.VTServerImpl()");
        try {
            votes= new VoteTable("votes.dat");
        } catch (IOException e) {
            e.printStackTrace();
            System.out.println(
                "Fehler in VTServerImpl.VTServerImpl() > " + e.getMessage());
        }
        System.out.println("<= VTServerImpl.VTServerImpl()");
    }

    /**
     * Update aller registrierten Clients. Wenn ein Client nicht 
     * erreichbar ist, wird er aus der Liste entfernt.
     */
    public synchronized void updateClients() {
        System.out.println("=> VTServerImpl.updateClients()");
        Enumeration elem= remoteObjects.elements();
        while (elem.hasMoreElements()) {
            VTClient client = null;
            RemoteObject remoteObject;
            try {
                remoteObject= (RemoteObject) elem.nextElement();
                client= (VTClient) remoteObject.getApp();
                System.out.println("try update \n\t" + client + "\n");
                client.update(votes);
            } catch (RemoteException e) {
                e.printStackTrace();
                System.out.println(
                    "Fehler in VTServerImpl.updateClients() "
                        + "update fehlgeschlagen:\n\t"
                        + client
                        + " > "
                        + e.getMessage());
                remoteObjects.remove(client);
            } catch (ClassCastException e) {
                e.printStackTrace();
                System.out.println(
                    "Fehler in VTServerImpl.updateClients() "
                        + "update fehlgeschlagen:\n\t"
                        + client
                        + " > "
                        + e.getMessage());
                remoteObjects.remove(client);
            }
        }
        System.out.println("<= VTServerImpl.updateClients()");
    }

    /**
     * Registrierung des Clients. Ruft auf dem Client die update() 
     * Methode auf und übergibt im somit die derzeitigen Daten.
     * @param projects.projects.Client - der registriert werden soll.
     * @throws <{RemoteException}>
    */
    public synchronized String register(RemoteObject remoteObject)
        throws RemoteException {
        System.out.println("=> VTServerImpl.register()");
        String result= super.registerComponent(remoteObject);
        VTClient app= (VTClient) remoteObject.getApp();
        app.update(votes);
        System.out.println("Status " + result + " | client: " + app);
        System.out.println("<= VTServerImpl.register()");
        return result;
    }

    /**
     * Abgabe eines Votes.
     * @param voteid - des gewählten Votes.
     */
    public synchronized void vote(String voteid) throws RemoteException {
        if (votes.containsKey(voteid)) {
            Vote vote= (Vote) votes.get(voteid);
            vote.setCount(vote.getCount() + 1);
        }
        this.updateClients();
    }
}
