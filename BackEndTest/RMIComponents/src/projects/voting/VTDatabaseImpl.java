/*
 * Created on 15.08.2004
 *
 * To change the template for this generated file go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
package projects.voting;

import java.rmi.RemoteException;
import java.util.List;
import java.util.Vector;

import net.sf.hibernate.HibernateException;
import net.sf.hibernate.Session;
import net.sf.hibernate.SessionFactory;
import net.sf.hibernate.Transaction;
import projects.interfaces.VTDatabaseServer;
import projects.voting.model.DBVote;
import API.control.Database;
import API.model.RemoteObject;

/**
 * @author tobi
 *
 * To change the template for this generated type comment go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
public class VTDatabaseImpl extends Database implements VTDatabaseServer {

	private SessionFactory sessionFactory;

	/**
	 * 
	 */
	public VTDatabaseImpl() throws RemoteException {
		System.out.println("=>VTDatabaseImpl.constructor\n");
		System.out.println(
			"VTDatabaseServer starten mit der in der properties spezialiseierten Datenbank");
		//Server hsqlserver = new Server();
		System.out.println("\n<= VTDatabaseImpl.constructor\n");
	}

	/* (non-Javadoc)
	 * @see API.interfaces.ServerHandle#register(API.model.RemoteObject)
	 */
	public String register(RemoteObject service) throws RemoteException {
		// TODO Auto-generated method stub
		return null;
	}

	/**
	* 	
	*/
	public void storeVotes(Vector dataVector) {
		try {
			Session session = sessionFactory.openSession();
			Transaction tx = session.beginTransaction();

			for (int i = 0; i < dataVector.size(); i++) {
				DBVote vote = (DBVote) dataVector.get(i);
				session.saveOrUpdate(vote);				
			}
			tx.commit();
			session.close();
		} catch (HibernateException e) {
			e.printStackTrace();
		}
	}
	
	/**
		* Speichert einzelne DBVotes in der Datenbank
		* @param title		
		*/
		public void storeVote(DBVote vote) {
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

	private List listVotes() {
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
	 * @see projects.interfaces.VTLogicServer#getDatabaseTable()
	 */
	public void getDatabaseTable() {
		// TODO Auto-generated method stub

	}

}
