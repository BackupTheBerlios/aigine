/*
 * Created on Aug 11, 2004
 *
 * To change the template for this generated file go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
package API.control;

import java.rmi.RemoteException;
import java.rmi.server.RMIClientSocketFactory;
import java.rmi.server.RMIServerSocketFactory;

import net.sf.hibernate.HibernateException;
import net.sf.hibernate.SessionFactory;
import net.sf.hibernate.cfg.Configuration;
import API.model.RemoteObject;


/**
 * @author tobi
 *
 * Project : RMIComponents, At : Aug 11, 2004
 */
public abstract class Database extends Server {

	private SessionFactory sessionFactory;
	private Configuration dbVoteConfig;

	/**
	 * @throws RemoteException
	 */
	public Database() throws RemoteException {
		super();
//		Vote dbvote = new Vote();
		//System.out.println("Vote wird mal vorgeladen... mit der id "+ dbvote.getId());		
		Configuration cfg = new Configuration();
		dbVoteConfig = cfg;
		System.out.println("Initializing Hibernate");
		//dbVoteConfig = new Configuration();
//		URL url = null;
//		try {
//			url = new URL(sessionConfigURL);
//			System.out.println("reding the first url--------------------------1");
//			dbVoteConfig = cfg.addURL(url);			
//		} catch (MappingException e1) {
//			System.out.println(
//			"\n\n<<<<<<<<<<<<<<<<<<your mamas foult>>>>>>>>>>>>>>>>>>>\n\n" +//			"the given path : " + url.getPath()+" | given port:"+url.getPort()+" | given file "+url.getFile());
//			System.out.println(
//				"Fehler in "
//					+ this.getClass().getName()
//					+ " => "
//					+ e1.getMessage());			
//		} catch (MalformedURLException e1) {
//			System.out.println(
//				"Fehler in "
//					+ this.getClass().getName()
//					+ " => "
//					+ e1.getMessage());
//		}
		try {
			System.out.println("trying to create sessionFactory ---------*");
			sessionFactory = dbVoteConfig.configure().buildSessionFactory();
			System.out.println("Finished Initializing Hibernate.");
		} catch (HibernateException e) {
			e.printStackTrace();
		}
		System.out.println("SessionFactory has been builded! sf = " +sessionFactory.toString());
		// TODO Datenbank starten

	}

	/**
	 * @param port
	 * @throws RemoteException
	 */
	public Database(int port) throws RemoteException {
		super(port);
	}

	/**
	 * @param port
	 * @param csf
	 * @param ssf
	 * @throws RemoteException
	 */
	public Database(
		int port,
		RMIClientSocketFactory csf,
		RMIServerSocketFactory ssf)
		throws RemoteException {
		super(port, csf, ssf);
	}

	/* Register Methode des Servers mit Authentifizierung
	 * @see API.control.Server#register(API.model.RemoteObject, java.lang.String, java.lang.String)
	 */
	public synchronized String register(
		RemoteObject remoteObject,
		String usr,
		String pass)
		throws RemoteException {
		return super.register(remoteObject, usr, pass);
	}

	/* Register Methode des Servers als anonnymer client
		 * @see API.control.Server#register(API.model.RemoteObject, java.lang.String, java.lang.String)
		 */
	public synchronized String register(RemoteObject remoteObject) {
		return super.registerComponent(remoteObject);
	}

	/**
	 * @return
	 * Database.java
	 */
	public Configuration getDbVoteConfig() {
		return dbVoteConfig;
	}

	/**
	 * @return
	 * Database.java
	 */
	public SessionFactory getSessionFactory() {
		return sessionFactory;
	}

}
