/*
 * Created on Aug 11, 2004
 *
 * 
 */
package API.control;

import java.rmi.RemoteException;
import java.rmi.server.RMIClientSocketFactory;
import java.rmi.server.RMIServerSocketFactory;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Properties;

import API.model.AbstractDAO;
import API.model.AbstractDTO;
import API.model.RemoteObject;

import org.postgresql.Driver;

//import org.postgresql.jdbc3.Jdbc3PoolingDataSource;

/**
 * Dieser abstrakte Server bietet die grundlegende Funktionalität für DBMS-Ansprechende, JDBC-basierende Einzelprozesse.
 * Diese bilden die Schnittstelle zwischen den DTO-Objekten im normalen RMI-Netz und den JDBC-Aufrufen an das DBMS.
 * TODO Dieser Server ist nach dem neuen Modell eine Komponente
 * @author Dennis
 *
 * Project : RMIComponents, At : Aug 11, 2004
 */
/* Worker-Objekt, wird der Methode übergeben und hat dann jeweils eine indivduelle Implemtierung der inneren Methode. */

public abstract class DbServer extends Server {
	private Connection con = null ;
	
	final public int DUMMY_DAO = -1 ;
//	private ArrayList theDAOs = new ArrayList() ;
	private AbstractDAO[] theDAOs2 = null ;
	private String dbname = null ;
	private String dbuser = null ;
	private String dbpasswd = null ;
	
	
	protected void protokoll(String meldung) {
		/* Dies ist eine Dummyfunktion. Sie sollte durch eine Methode der Superklasse
		 * Component ersetzt werden. Um die Umstellung zu erleichtern, werde ich an
		 * allen wichtigen Stellen bereits diese Methode aufrufen.
		 */
		 System.out.println(meldung) ;
	}
	
/*	protected void addDAO() {
		theDAOs.add()
	} */
	
	/**
	 * Diese Methode erzeugt ein AbstractDAO[] Array, welches die DbServer DAOs enthält.
	 * Deren Konstruktoren muss jeweils die Connection übergeben werden!!!
	 */
	abstract protected AbstractDAO[] initDAOList() ;
	abstract protected void initConnectionValues() ;
	
	private AbstractDAO selectRegisteredDAO(int whichDAO) {
		if (whichDAO < theDAOs2.length) {
			return theDAOs2[whichDAO] ;
		}
		return null;
	}
	
	public final int insert(int whichDAO, AbstractDTO theDTO) throws RemoteException {
		AbstractDAO theDAO = this.selectRegisteredDAO(whichDAO) ;
		if (theDAO != null) {
			try {
				theDAO.insert(theDTO) ;
			} catch (SQLException sqle) {
				this.protokoll("Es ist eine SQLException bei INSERT aufgetreten:\n" +
					sqle.getMessage()) ;
			}
		}
		return -1 ;
	}
	
	public int update(int whichDAO, AbstractDTO theDTO, Properties parameter) throws RemoteException {
		AbstractDAO theDAO = this.selectRegisteredDAO(whichDAO) ;
		if (theDAO != null) {
			try {
				theDAO.update(theDTO) ;
			} catch (SQLException sqle) {
				this.protokoll("Es ist eine SQLException bei UPDATE aufgetreten:\n" +
					sqle.getMessage()) ;
			}
		}
		return -1 ;
	}
	
	public AbstractDTO[] loadList(int whichDAO, Properties parameter) throws RemoteException {
		AbstractDAO theDAO = this.selectRegisteredDAO(whichDAO) ;
		AbstractDTO[] theDTOs = null ;
		if (theDAO != null) {
			try {
				theDTOs = theDAO.loadList(parameter) ;
			} catch (SQLException sqle) {
				this.protokoll("Es ist eine SQLException bei loadList() aufgetreten:\n" +
					sqle.getMessage()) ;
			}
		}
		return theDTOs ;
	}
	
	public AbstractDTO load(int whichDAO, Properties parameter) throws RemoteException {
		AbstractDAO theDAO = this.selectRegisteredDAO(whichDAO) ;
		AbstractDTO theDTO = null ;
		if (theDAO != null) {
			try {
				theDTO = theDAO.load(parameter) ;
			} catch (SQLException sqle) {
				this.protokoll("Es ist eine SQLException bei load() aufgetreten:\n" +
					sqle.getMessage()) ;
			}
		}
		return theDTO ;
	}
	
	public int delete(int whichDAO, Properties parameter) throws RemoteException {
		// TODO byDR tut derzeit nichts
		return -1 ;
	}



	private void initConnection(String dbname, String username, String passwd) {
//		dataSource = new Jdbc3PoolingDataSource();
		
		try {
			Class.forName("org.postgresql.Driver");
		} catch (ClassNotFoundException cnfe) {
//			hier Protokollfunktion verwenden
			this.protokoll("--- Die JDBC-Treiberklasse des Datenbankservers konnte" +
			" nicht gefunden werden!!!\n" + cnfe.getStackTrace()) ;
			System.exit(5) ;
		}
		
		String url = new String("jdbc:postgresql://localhost/" + dbname) ;
		try {
			con = DriverManager.getConnection(url, username, passwd);
		} catch (SQLException sqle) {
			sqle.printStackTrace() ;
			this.protokoll("--- Der Aufbau einer Connection ist mit folgender SQLException gescheitert:" +				sqle.getStackTrace() ) ;
		}
	}

	/**
	 * @throws RemoteException
	 */
	public DbServer() throws RemoteException {
		super();
		this.protokoll("==> Standardkonstruktor wurde aufgerufen ... versuche nun die db-Verbindung zu initialisieren ...") ;
		this.dbname = "portaldb" ;
		this.initConnectionValues() ;
		this.initConnection(dbname, dbuser, dbpasswd) ;
		this.theDAOs2 = this.initDAOList() ;
//		this.initConnection("portaldb", "postgres", "postgres") ;
	}
	
	/**
	 * @param string
	 */
	public void setDbname(String string) {
		dbname = string;
	}

	/**
	 * @param string
	 */
	public void setDbpasswd(String string) {
		dbpasswd = string;
	}

	/**
	 * @param string
	 */
	public void setDbuser(String string) {
		dbuser = string;
	}
	

	/* Die nachfolgenden Methoden sind nur die notwendigen abstrakten Implementierungen,
	 * die sich aus der Abstammung von Server ergeben. Sie werden weitgehend rausfallen. */


	/**
	 * @param port
	 * @throws RemoteException
	 */
	public DbServer (int port) throws RemoteException {
		super(port);
	}

	/**
	 * @param port
	 * @param csf
	 * @param ssf
	 * @throws RemoteException
	 */
	public DbServer(
		int port,
		RMIClientSocketFactory csf,
		RMIServerSocketFactory ssf)
		throws RemoteException {
		super(port, csf, ssf);
	}

	/* Register Methode des Servers mit Authentifizierung
	 * @see API.control.Server#register(API.model.RemoteObject, java.lang.String, java.lang.String)
	 */
	public synchronized String register(RemoteObject remoteObject, String usr, String pass)
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
	 */
	public Connection getCon() {
		return con;
	}

	/**
	 * @param connection
	 */
	public void setCon(Connection connection) {
		con = connection;
	}

}
