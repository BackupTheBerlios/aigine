package projects.voting.control;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Date;

import javax.swing.JLabel;
import javax.swing.table.DefaultTableModel;

import projects.interfaces.VTLogicServer;
import API.model.RemoteObjectTable;


/**
 * ActionListener für die Abgabe eines Votes.
 * @author danny
 * @since 07.05.2004 14:17:18
 */
public class VTAdminClientRefreshAction implements ActionListener {

	private VTLogicServer server = null;
	private JDataTable managedObjects = null;
	private JLabel statuslabel = null;

	/**
	 *  Konstruktor
	 * @param server Objekt für den Serverzugriff.
	 */
	public VTAdminClientRefreshAction(VTLogicServer server, 
		JDataTable managedObjects, JLabel statuslabel) {
		super();
		this.server = server;		
		this.statuslabel = statuslabel;
		this.managedObjects = managedObjects;
	}

	/**
	 * @param ActionEvent
	 */
	public void actionPerformed(ActionEvent ae) {
		try {
			// TODO Beim Server ein update der Daten anfordern.			
			RemoteObjectTable rot = server.checkClientConnections();
			DefaultTableModel model = (DefaultTableModel)managedObjects.getModel();
			model.setDataVector(rot.getDataVector(), rot.getKeyVector());
			// TODO Tabelle muss beim ersten Start die ermittelten Breiten speichern,
			// um ständigen Neuaufbau zu vermeiden.
			managedObjects.resizeTableColumns();
			statuslabel.setText("refreshed list : " + new Date());
		} catch ( Exception e ) {
			e.printStackTrace();
			System.out.println("Fehler beim Voting!");
		}
	}
}
