/*
 * Created on 15.08.2004
 *
 * To change the template for this generated file go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
package projects.voting.control;

import java.net.URI;
import java.net.URISyntaxException;
import java.util.Enumeration;
import java.util.List;

import net.sf.hibernate.HibernateException;
import net.sf.hibernate.Session;
import net.sf.hibernate.SessionFactory;
import net.sf.hibernate.Transaction;
import projects.interfaces.VTPersistenceHelper;
import projects.voting.model.Vote;
import projects.voting.model.VoteTable;
import API.interfaces.ServerHandle;

/**
 * @author tobi
 * 
 * To change the template for this generated type comment go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
public class HelperHibernatePersistence implements VTPersistenceHelper {

	private SessionFactory sessionFactory;

	//Vorsicht hier steht auch immer die "general" mit drin!
	private String[] categorynames;

	private String databasename;

	private VoteTable votes;

	/**
	 *  
	 */
	public HelperHibernatePersistence(SessionFactory sf,
			ServerHandle classServer) {
		System.out.println("=>HelperHibernatePersistence:Constructor");
		votes = new VoteTable();
		sessionFactory = sf;
		System.out.println("<=HelperDatabasePersistence.constructor");

	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see projects.voting.control.PersistenceHelper#refreshConfiguration()
	 */
	public void refreshConfiguration() {

	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see projects.voting.control.PersistenceHelper#save()
	 */
	public void save() {
		System.out.println("=>HleperHibernatePersistence.save()");
		try {

			Enumeration enum = votes.elements();
			System.out.println("Enumeration of votes: " + enum);
			while (enum.hasMoreElements()) {
				Session session = sessionFactory.openSession();
				System.out.println("Session is open: " + session);
				Transaction tx = session.beginTransaction();
				System.out.println("begin Transaction: " + tx);
				Vote vote = (Vote) enum.nextElement();
				System.out.println("zu speichernde vote: " + vote.toString());
				System.out.println("HelperDatabasePer.save: the next element="
						+ vote);
//				session.save(vote);
//				session.saveOrUpdateCopy(vote);
				session.saveOrUpdate(vote);
				tx.commit();
				session.close();
				System.out.println("vote saved!");
			}

		} catch (HibernateException e) {
			e.printStackTrace();
		}
		System.out.println("<=HleperHibernatePersistence.save()");
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

	/*
	 * (non-Javadoc)
	 * 
	 * @see interfaces.PersistenceHelper#getVoteTable()
	 */
	public VoteTable getVoteTable() {
		System.out.println("=>HleperHibernatePersistence.getVoteTable()");
		VoteTable newVotes = null;
		Session session = null;
		Transaction tx = null;
		try {
			System.out.println("session wird geoeffnet...");
			session = sessionFactory.openSession();
			System.out.println("....open!");
			tx = session.beginTransaction();
		
			System.out.println("....begin transaction...");
			List result = session.find("from Votes");
			System.out.println("gitting from database: " + result);
			for (int i = 0; i < result.size(); i++) {
				Vote dbv = (Vote) result.get(i);
				newVotes.put(String.valueOf(dbv.getId()), (Vote) dbv);
			}

			

		} catch (HibernateException e) {
			throw new RuntimeException(e.getMessage());
		} finally {
			try {
				tx.commit();
				session.close();
			}catch (HibernateException e) {
				
			}
		}
		System.out.println("<=HleperHibernatePersistence.getVoteTable()");
		return newVotes;
	}

	/**
	 * checkt ob die uebergebene URI koreckt angegben wurde
	 * 
	 * @param uri
	 *            HelperDatabasePersistence.java
	 */
	public URI uriParser(String uriString) {
		try {
			URI uri = new URI("file", "/projects/voting/model/Vote.hbm.xml",
					null);
			System.out.println("Schema   : " + uri.getScheme()
					+ "\nSchemaspezifischer Teil: "
					+ uri.getSchemeSpecificPart() + "\nFragment : "
					+ uri.getFragment());
			if (!uri.isOpaque())
				System.out.println("Autoritaet: " + uri.getAuthority()
						+ "\nPfad     : " + uri.getPath() + "\nAnfrage  : "
						+ uri.getQuery() + "\nUser     : " + uri.getUserInfo()
						+ "\nHost     : " + uri.getHost() + "\nPort     : "
						+ uri.getPort());
			return uri;
		} catch (URISyntaxException e) {
			System.out.println("URI nicht korrekt gebildet." + e.getMessage());
		}
		System.out.println("\n URI ' " + uriString + " ist korrekt");
		return null;

	}

	/**
	 * 
	 * @param uri
	 * @return Valid URI to go HelperDatabasePersistence.java
	 */
	public void uriParser(URI uri) {

		System.out.println("Schema   : " + uri.getScheme()
				+ "\nSchemaspezifischer Teil: " + uri.getSchemeSpecificPart()
				+ "\nFragment : " + uri.getFragment());
		if (!uri.isOpaque())
			System.out.println("Autoritaet: " + uri.getAuthority()
					+ "\nPfad     : " + uri.getPath() + "\nAnfrage  : "
					+ uri.getQuery() + "\nUser     : " + uri.getUserInfo()
					+ "\nHost     : " + uri.getHost() + "\nPort     : "
					+ uri.getPort());
		System.out.println("\n URI ' " + uri + " ist korrekt");

	}

	/**
	 * @param votes
	 *            The votes to set.
	 */
	public void setVoteTable(VoteTable votes) {
		this.votes = votes;
	}
}