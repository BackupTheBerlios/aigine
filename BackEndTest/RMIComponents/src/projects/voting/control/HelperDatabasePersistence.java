/*
 * Created on 15.08.2004
 *
 * To change the template for this generated file go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
package projects.voting.control;

import org.jconfig.Configuration;

import projects.interfaces.VTDatabaseServer;
import projects.voting.model.VoteTable;
import API.interfaces.ServerHandle;

/**
 * @author tobi
 *
 * To change the template for this generated type comment go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
public class HelperDatabasePersistence {
	
	private Configuration configuration;
	 //Vorsicht hier steht auch immer die "general" mit drin!
	 private String[] categorynames;
	 private String databasename;
	 private VoteTable votes;

	/**
	 * 
	 */
	public HelperDatabasePersistence(String dbname, ServerHandle dbserver) {
		votes = new VoteTable();
		
		//TODO eine Conection zu dem Datenbank Server aufbauen. dieser oeffnet eine neue SessionFactory
		//als naechstes dessen methoden fuer die gewuenschten daten aufrufen ( getKompleteVotes, getVote, saveVote, saveAllVotes...)
		
		VTDatabaseServer hsqlDB = (VTDatabaseServer) dbserver; 
		
		
	}

}