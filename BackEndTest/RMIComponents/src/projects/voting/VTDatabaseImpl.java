/*
 * Created on 15.08.2004
 *
 * To change the template for this generated file go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
package projects.voting;

import java.rmi.RemoteException;
import java.util.List;

import projects.interfaces.VTDatabaseServer;
import projects.interfaces.VTLogicServer;
import projects.interfaces.VTPersistenceHelper;
import projects.voting.control.HelperHibernatePersistence;
import projects.voting.control.HelperXmlPersitence;
import projects.voting.model.Vote;
import projects.voting.model.VoteTable;
import API.control.Database;
import API.interfaces.ServerHandle;
import API.model.RemoteObject;









/**
 * @author tobi
 *
 * To change the template for this generated type comment go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
public class VTDatabaseImpl extends Database implements VTDatabaseServer {
	
	
	private VTPersistenceHelper storehelper;

	private VoteTable votes;

	private VTLogicServer logicserver;

	/**
	 * 
	 */
	public VTDatabaseImpl() throws RemoteException {
		super();
		
		System.out.println("=>VTDatabaseImpl.constructor\n");
		System.out.println(
			"VTDatabaseServer starten mit der in der properties spezialiseierten Datenbank");
		//storehelper = new HelperXmlPersitence("shellvotes");		
		storehelper = (VTPersistenceHelper) new HelperHibernatePersistence( super.getSessionFactory(), this);
		this.votes = new HelperXmlPersitence("shellvotes").getVoteTable();
		//this.votes = this.getVoteTable();
		System.out.println("Vote table: \n"+this.votes.toString());
		System.out.println("save aufruf!!");
		storehelper.setVoteTable(votes);
		this.save();
		System.out.println("starting pluged in DBManagerswing...");
		//DatabaseManagerSwing.main()
		System.out.println("\n<= VTDatabaseImpl.constructor\n");
	}

	/* (non-Javadoc)
	 * @see API.interfaces.ServerHandle#register(API.model.RemoteObject)
	 */
	public String register(RemoteObject service) {
		System.out.println("=>VTDatabaseImpl.register() > " + service);
		// TODO Service regestrierung implementieren!
		System.out.println("<=VTDatabaseImpl.register() > " + service);
		
		return super.register(service);
	}

	

	/**
		* Speichert einzelne Votes in der Datenbank
		* @param title		
		
	public void storeVote(Vote vote) {
		try {
			Session session = sessionFactory.openSession();
			Transaction tx = session.beginTransaction();
			session.saveOrUpdate(vote);
			tx.commit();
			session.close();
		} catch (HibernateException e) {
			e.printStackTrace();
		}
	}
*/
	
	/**
	 * called the PersistensHelper class to manage the way of persistens
	 */
	public void save() {
		storehelper.save();
	}
	
	/**
	 * 
	 */
	public List listVotes() {		
		List votelist = null;
		try {
			votelist = storehelper.listVotes();
		} catch (RemoteException e) {
			System.out.println(
				"Fehler in " + this.getClass().getName() + " => " + e.getMessage());
		}
		return votelist;
		
	}

    /* (non-Javadoc)
     * @see projects.interfaces.VTDatabaseServer#getVoteTable()
     */
    public VoteTable getVoteTable() throws RemoteException {
		System.out.println("=>VTDatabaseImpl.getVoteTable()");
		
		votes = storehelper.getVoteTable();
		if (votes == null) {
			System.out.println("Keine DB am Start___________________________________!!!!!!!!!!! :((");
		}

        return votes;
        
    }
	/**
	 * Registrierung eines Logik Service
	 * verwendet, um eine mögliche Überblendung zu vermeiden. 
	 * @param service
	 * @throws RemoteException
	 */
	public synchronized String registerService(final RemoteObject remoteObject, ServerHandle service) throws RemoteException{
		return super.registerService(remoteObject);
	}

    /* (non-Javadoc)
     * @see projects.interfaces.VTDatabaseServer#refreshConfiguration()
     */
    public void refreshConfiguration() {
		storehelper.refreshConfiguration();
        
    }
    /**
     * @return
     */
    public VTPersistenceHelper getStorehelper() {
        return storehelper;
    }

    /**
     * @param helper
     */
    public void setStorehelper(VTPersistenceHelper helper) {
        storehelper= helper;
    }
    
    
    
	/**
	 * Abgabe eines Votes.
	 * @param voteid - des gewählten Votes.
	 */
	public synchronized void vote(String voteid) throws RemoteException {
		System.out.println("=> VTDatabaseImpl.vote()");
		if (votes.containsKey(voteid)) {
			Vote vote= (Vote)votes.get(voteid);
			vote.setCount(vote.getCount() + 1);
			save();
		}
		System.out.println("<= VTDatabaseImpl.vote() > new votes: " + votes);
	}

	/* 
	 * update methode um den dbServer wie ein client haendelbar zu mahchen 
	 * @see projects.interfaces.VTDatabaseServer#update(projects.voting.model.VoteTable)
	 */
	public void update(VoteTable votes) {
		System.out.println("=>VTDatabaseImpl.update(votes)");
		System.out.println("givin votes:\n"+votes);
		this.votes = votes;
		storehelper.setVoteTable(votes);
		this.save();
		System.out.println("new votetable saved");
		System.out.println("<=VTDatabaseImpl.update(votes)");
	}
}
