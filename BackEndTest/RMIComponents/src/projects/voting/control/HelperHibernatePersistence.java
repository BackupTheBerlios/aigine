/*
 * Created on 15.08.2004
 *
 * To change the template for this generated file go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
package projects.voting.control;

import java.net.MalformedURLException;
import java.net.URI;
import java.net.URISyntaxException;
import java.net.URL;
import java.util.Enumeration;
import java.util.List;

import net.sf.hibernate.HibernateException;
import net.sf.hibernate.Session;
import net.sf.hibernate.SessionFactory;
import net.sf.hibernate.Transaction;
import net.sf.hibernate.cfg.Configuration;

import projects.interfaces.VTPersistenceHelper;
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
public class HelperHibernatePersistence implements VTPersistenceHelper {

	private SessionFactory sessionFactory;
	//Vorsicht hier steht auch immer die "general" mit drin!
	private String[] categorynames;
	private String databasename;
	private VoteTable votes;

	/**
	 * 
	 */
	public HelperHibernatePersistence(
		
		SessionFactory sf,
		ServerHandle classServer) {
		System.out.println("=>HelperHibernatePersistence:Constructor");
		votes = new VoteTable();
		sessionFactory = sf;
		System.out.println(
			"<=HelperDatabasePersistence.constructor");

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
			while (enum.hasMoreElements()) {

				DBVote vote = (DBVote) enum.nextElement();
				System.out.println(
					"HelperDatabasePer.save: the next element=" + vote);
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
		VoteTable newVotes = null;
		try {
			Session session = sessionFactory.openSession();
			Transaction tx = session.beginTransaction();

			List result = session.find("from Votes");
			for (int i = 0; i < result.size(); i++) {
				DBVote dbv = (DBVote) result.get(i);
				newVotes.put(dbv.getId(), (Vote) dbv);
			}

			tx.commit();
			session.close();

			return newVotes;
		} catch (HibernateException e) {
			throw new RuntimeException(e.getMessage());
		}
	}

	/**
	 * checkt ob die uebergebene URI koreckt angegben wurde
	 * @param uri 
	 * HelperDatabasePersistence.java
	 */
	public URI uriParser(String uriString) {
		try {
			URI uri =
				new URI("file", "/projects/voting/model/DBVote.hbm.xml", null);
			System.out.println(
				"Schema   : "
					+ uri.getScheme()
					+ "\nSchemaspezifischer Teil: "
					+ uri.getSchemeSpecificPart()
					+ "\nFragment : "
					+ uri.getFragment());
			if (!uri.isOpaque())
				System.out.println(
					"Autoritaet: "
						+ uri.getAuthority()
						+ "\nPfad     : "
						+ uri.getPath()
						+ "\nAnfrage  : "
						+ uri.getQuery()
						+ "\nUser     : "
						+ uri.getUserInfo()
						+ "\nHost     : "
						+ uri.getHost()
						+ "\nPort     : "
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
	 * @return Valid URI to go
	 * HelperDatabasePersistence.java
	 */
	public void uriParser(URI uri) {

		System.out.println(
			"Schema   : "
				+ uri.getScheme()
				+ "\nSchemaspezifischer Teil: "
				+ uri.getSchemeSpecificPart()
				+ "\nFragment : "
				+ uri.getFragment());
		if (!uri.isOpaque())
			System.out.println(
				"Autoritaet: "
					+ uri.getAuthority()
					+ "\nPfad     : "
					+ uri.getPath()
					+ "\nAnfrage  : "
					+ uri.getQuery()
					+ "\nUser     : "
					+ uri.getUserInfo()
					+ "\nHost     : "
					+ uri.getHost()
					+ "\nPort     : "
					+ uri.getPort());
		System.out.println("\n URI ' " + uri + " ist korrekt");

	}
}