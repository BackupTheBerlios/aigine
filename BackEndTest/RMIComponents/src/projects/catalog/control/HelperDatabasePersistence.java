/*
 * Created on Aug 28, 2004
 *
 * To change the template for this generated file go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
package projects.catalog.control;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.Properties;

import org.hsqldb.Server;

import projects.catalog.model.Picture;

import API.interfaces.PersistenceHelper;

/**
 * @author tobi your mama is Project : RMIComponents, At : Aug 28, 2004
 */
public class HelperDatabasePersistence implements PersistenceHelper {

	//	TODO eine Conection zu dem Datenbank Server aufbauen. dieser oeffnet eine
	// neue SessionFactory
	//als naechstes dessen methoden fuer die gewuenschten daten aufrufen

	//  change the url to reflect your preferred db location and name
	//  String url = "jdbc:hsqldb:hsql://localhost/yourtest";
	private String serverProps;

	private String url;

	private String user = "sa";

	private String password = "";

	private Server server;

	private boolean isNetwork = true;

	private boolean isServer;

	private Server dbserver;

	private String dbname;
	//das zu sichernde object
	private Object data;

	private Statement stmnt;

	private PreparedStatement pstmnt;

	private Connection connection;

	/**
	 *  
	 */
	public HelperDatabasePersistence(String dbname, Object data) {
		this.data = data;
		this.dbname = dbname;
		this.isServer = false;
	}

	protected void setUp(Properties serverProp) {

		if (isNetwork) {
			serverProps = "database.0=mem:" + dbname;
			url = "jdbc:hsqldb:hsql://localhost";
			server = new Server();

			server.putPropertiesFromString(serverProps);
			server.setLogWriter(null);
			server.setErrWriter(null);
			server.start();
		} else {
			url = "jdbc:hsqldb:file:" + dbname;
		}

		try {
			Class.forName("org.hsqldb.jdbcDriver");
		} catch (Exception e) {
			e.printStackTrace();
			System.out.println(this + ".setUp()database:" + dbname + " error: "
					+ e.getMessage());
		}
		isServer = true;
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see API.interfaces.PersistenceHelper#save()
	 */
	public void save() {
		// TODO speicher ein db abbild des data- objectes
		if (isServer) {
			try {
				connection = newConnection();
				stmnt = connection.createStatement();
			} catch (Exception e) {
			}
		}

	}

	/**
	 * persist the Catalog object
	 *  
	 */
	private void saveNewCatalog() {

	}

	private void createDataSructure() {
		if (isServer) {

			try {
				connection = newConnection();
				stmnt = connection.createStatement();
			} catch (Exception e) {

				e.printStackTrace();
			}
			String drop = "DROP TABLE USER IF EXISTS; "
					+ "DROP TABLE PICTURE IF EXISTS;"
					+ "DROP TABLE CATALOG IF EXISTS;"
					+ "DROP TABLE SUBCATALOG IF EXISTS;"
					+ "DROP TABLE WAHRENKORB IF EXISTS";
			String create = "CREATE TABLE USER(USER_ID INTEGER NOT NULL PRIMARY KEY,"
					+ "LOGIN_ID VARCHAR(128) NOT NULL,"
					+ "USER_NAME VARCHAR(254) DEFAULT ' ' NOT NULL,"
					+ "CREATE_DATE TIMESTAMP DEFAULT CURRENT_TIMESTAMP NOT NULL,"
					+ "UPDATE_DATE TIMESTAMP DEFAULT CURRENT_TIMESTAMP NOT NULL,"
					+ "LAST_ACCESS_DATE TIMESTAMP,"
					+ "CONSTRAINT IXUQ_LOGIN_ID0 UNIQUE(LOGIN_ID))";

			try {
				stmnt.execute(drop);
			} catch (SQLException e1) {

				System.out.println("Error: exception type: SQLException"
						+ "with the message: \n " + e1.getMessage());

			}

		}
	}

	public void savePic() {
		Picture pic = (Picture) this.data;

	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see API.interfaces.PersistenceHelper#refreshConfiguration()
	 */
	public void refreshConfiguration() {
		// TODO Auto-generated method stub
	}

	/**
	 * shutdown the running DB server
	 *  
	 */
	protected void tearDown() {

		if (isNetwork) {
			server.stop();
			server = null;
			isServer = false;
		}
	}

	Connection newConnection() throws Exception {
		return DriverManager.getConnection(url, user, password);
	}

}