/*
 * Created on 15.08.2004
 *
 * To change the template for this generated file go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
package projects.voting.control;

import interfaces.PersistenceHelper;

import java.rmi.RemoteException;
import java.util.Enumeration;
import java.util.List;

import net.sf.hibernate.HibernateException;
import net.sf.hibernate.Session;
import net.sf.hibernate.SessionFactory;
import net.sf.hibernate.Transaction;
import net.sf.hibernate.cfg.Configuration;
import projects.interfaces.VTDatabaseServer;
import projects.voting.model.DBVote;
import projects.voting.model.Vote;
import projects.voting.model.VoteTable;
import API.interfaces.ServerHandle;

/**
 * @author tobi
 *
 * To change the template for this generated type comment go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
public class HelperDatabasePersistence implements PersistenceHelper {
	
	private SessionFactory sessionFactory;
	 //Vorsicht hier steht auch immer die "general" mit drin!
	 private String[] categorynames;
	 private String databasename;
	 private VoteTable votes;

	/**
	 * 
	 */
	public HelperDatabasePersistence(String dbname, ServerHandle dbserver) {
		votes = new VoteTable();
		
		try {
					System.out.println("Initializing Hibernate");
					sessionFactory = new Configuration().configure().buildSessionFactory();
					System.out.println("Finished Initializing Hibernate");
				} catch (HibernateException e) {
					e.printStackTrace();
				}
		
		//TODO eine Conection zu dem Datenbank Server aufbauen. dieser oeffnet eine neue SessionFactory
		//als naechstes dessen methoden fuer die gewuenschten daten aufrufen ( getKompleteVotes, getVote, saveVote, saveAllVotes...)
		
		VTDatabaseServer hsqlDB = (VTDatabaseServer) dbserver; 
		try {
			List votelist = hsqlDB.listVotes();
		} catch (RemoteException e) {
			System.out.println("HelperDatabasePersistence.constructor. listVotes of VTDatabaseServer faild!!");
			e.printStackTrace();
		}
		System.out.println("HelperDatabasePersistence.constructor. listVotes!!! <<<<------------please call me 'master of it all'---------->>>>!!!!");
		DBVote dbvote = new DBVote();
				
	}
	
	
	

	/* (non-Javadoc)
	 * @see projects.voting.control.PersistenceHelper#refreshConfiguration()
	 */
	public void refreshConfiguration() {
		
	}

	/* (non-Javadoc)
	 * @see projects.voting.control.PersistenceHelper#save()
	 */
	public void save() {
		try {
			   Session session = sessionFactory.openSession();
			   Transaction tx = session.beginTransaction();
               Enumeration enum = votes.elements();
               while(enum.hasMoreElements()){
               	
               
			   				DBVote vote = (DBVote) enum.nextElement();
							session.save(vote); 
               }
        
			   
        
			   tx.commit();
			   session.close();
		   } catch (HibernateException e) {
			   e.printStackTrace();
		   }
		
	}
	
	public List listVotes() {
			try {
				Session session = sessionFactory.openSession();
				Transaction tx = session.beginTransaction();

				List result = session.find("from Votes");

				tx.commit();
				session.close();

				return result;
			} catch (HibernateException e) {
				throw new RuntimeException(e.getMessage());
			}
		}




    /* (non-Javadoc)
     * @see interfaces.PersistenceHelper#getVoteTable()
     */
    public VoteTable getVoteTable() {
		VoteTable newVotes= null;
		try {
				 Session session = sessionFactory.openSession();
				 Transaction tx = session.beginTransaction();

				 List result = session.find("from Votes");
				 for (int i = 0; i < result.size(); i++) {
					DBVote dbv = (DBVote)result.get(i);
					newVotes.put(dbv.getId(),(Vote)dbv);
				 }
				 
				 
				
				 tx.commit();
				 session.close();

				 return newVotes;
			 } catch (HibernateException e) {
				 throw new RuntimeException(e.getMessage());
			 }
    }

}