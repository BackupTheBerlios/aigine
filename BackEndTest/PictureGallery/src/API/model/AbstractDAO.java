/*
 * Created on 16.03.2005
 *
 * @user drichter
 * */
package API.model;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.Enumeration;
import java.util.Properties;

/**
 * @author drichter
 * TODO Die Methode getRowNames() muss mehr als nur eine Kommagetrennte Liste liefern. Besser ist ein Array von Objekten, die neben ihrem
 * Namen auch ihren Typ wissen.
 * Diese Klasse stellt die Grundlage für die konkreten DAO-Klassen dar.
 */
public abstract class AbstractDAO {
	Connection con = null ;

	protected final String SQL_SELECT = "select * from " + getTableName();

	protected final String SQL_INSERT = "insert into " + getTableName() + " ("
			+ getRowNames() + ") Values";

	protected final String SQL_DELETE = "delete from " + getTableName();

	protected final String SQL_UPDATE = "update " + getTableName() + " set ("
			+ getRowNames() + ") = ";
	
	protected final String SQL_UPDATE_SHORT = "UPDATE "  + getTableName() + " SET " ;
	
	public AbstractDAO() {
		
	}
	
	public AbstractDAO(Connection theCon) {
		this.con = theCon ;
	}

	abstract protected String getTableName();

	abstract protected String getRowNames();

	abstract public void insert(AbstractDTO theDTO) throws SQLException ;
	
	abstract public void update(AbstractDTO theDTO) throws SQLException ;
	
	/**
	 * Diese Methode uebersetzt im DAO- und damit im Tabellenbezug die angebenen DTO-Variablennamen in Spaltennamen
	 * @param Var
	 * @return
	 */
	abstract protected String translateVarToCol(String var) ;
	
	/**
	 * Diese Methode soll DAOSpezifisch passende SubSelects aufbauen. Dabei muss sie eventuell in einer Abfrage zusammengehörende
	 * Parameterkombinationen erkennen - weshalb sie derzeit spezifisch implementiert werden muss.
	 * @param props
	 * @return
	 */
	abstract protected String generateSubSelect(Properties props) ;
	
	abstract public AbstractDTO load(Properties parameter) throws SQLException ;
	
	abstract public AbstractDTO[] loadList(Properties parameter) throws SQLException ;
	
	private String translateOperation(String opCode) {
		if (opCode.equals("EQUAL")) {
			return " = " ;
		} else if(opCode.equals("LIKE")) {
			return " LIKE " ;
		} else if(opCode.equals("DIFF")) {
			return " <> " ;
		} else if(opCode.equals("IN")) {
			return " IN " ;
		} else {
			return " = " ;
		}
	}
	
	protected String propToSQL(Properties props) {
		if (props.size() == 0)
			return "" ;
		
		StringBuffer theBuffer = new StringBuffer(" WHERE ") ;
		String keywOp = null ;
		String op = null ;
		String[] keyParts = null ; 
		Enumeration enum1 = props.keys() ;
		
		while (enum1.hasMoreElements()) {
			keywOp = new String((String) enum1.nextElement()) ;
			System.out.println("  > keywOp: " + keywOp) ;
			keyParts = keywOp.split("_") ;
			op = translateOperation(keyParts[1]) ;
			theBuffer.append(translateVarToCol(keyParts[0]) + op) ;
			if (op.equals("SUB")) {
				theBuffer.append(props.getProperty(keywOp)) ;
			} else {
				theBuffer.append(generateSubSelect(props)) ;
			}
			if (enum1.hasMoreElements())
				theBuffer.append(" and ") ;
		}
		return theBuffer.toString() ;
	}
	
	protected ResultSet basicLoad(Properties props) throws SQLException {
		StringBuffer sb = new StringBuffer(SQL_SELECT + this.propToSQL(props) + " ;") ;
		System.out.println("Dies ist mein fertiger SQL-Code: \n------------\t" + sb) ;
/*		if (con == null)
			System.out.println("\n--- Die Connection ist null!\n") ; */
		Statement stmnt = this.con.createStatement() ;
		ResultSet rs = stmnt.executeQuery(sb.toString()) ;
/*		if (rs != null)
			System.out.println("\n+++ Das ResultSet ist nicht leer!\n") ;
		else
			System.out.println("\n+++ Das ResultSet ist leer!\n") ; */
		return rs ;
	}
	
//	abstract protected AbstractDTO initDTO() throws SQLException ;
	 
//	abstract public void insert() ;

	/* Hier koennten noch ein paar allgemein sinnvolle Methoden folgen, sind aber
	 * erst einmal nicht notwendig. */
	
	
	/**
	 * @param connection
	 */
	public void setCon(Connection connection) {
		con = connection;
	}

}
