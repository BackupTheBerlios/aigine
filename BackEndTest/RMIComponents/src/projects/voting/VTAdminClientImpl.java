package projects.voting;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.rmi.RemoteException;

import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JScrollPane;
import javax.swing.table.DefaultTableModel;

import projects.interfaces.VTClient;
import projects.interfaces.VTLogicServer;
import projects.voting.control.JComponentCellEditor;
import projects.voting.control.JComponentCellRenderer;
import projects.voting.control.JDataTable;
import projects.voting.control.VTAdminClientRefreshAction;
import projects.voting.model.VoteTable;
import API.interfaces.AdminClient;
import API.interfaces.ServerHandle;
import API.model.RemoteObject;
import API.model.RemoteObjectTable;

/**
 * 
 * @author danny
 * @since 05.05.2004 20:16:24
 */
public class VTAdminClientImpl implements VTClient, AdminClient {

	private JFrame frame = new JFrame();
	private JLabel statuslabel = new JLabel("tschesch kollega");
	private JDataTable table = new JDataTable();
	private JButton bv = new JButton("refresh");
	private DefaultTableModel managedObjects = new DefaultTableModel();

	private int recievedCounter = 0;


	public void init(RemoteObject compProps, ServerHandle server) {
		System.out.println("=> VTAdminClientImpl.init(" +			"RemoteObjectProperties " + compProps + " , Object" + server);
		VTLogicServer s = (VTLogicServer) server;
		
		frame.setTitle("Admin Client for " + compProps.getCompName());
		setServerGrap(s);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.getContentPane().add(statuslabel, BorderLayout.SOUTH);
		
		// Tabelleninitialisierung
		JScrollPane tablepanel = new JScrollPane();
		table.setModel(managedObjects);
		table.setDoubleBuffered(true);
		table.setDefaultRenderer(
			JComponent.class,
			new JComponentCellRenderer());
		table.setDefaultEditor(
			JComponent.class,
			new JComponentCellEditor());
		tablepanel.setColumnHeaderView(table.getTableHeader());
		tablepanel.setViewportView(table);
		frame.getContentPane().add(tablepanel);
		frame.getContentPane().add(bv, BorderLayout.EAST);
		//Display the window.
		frame.setSize(new Dimension(1000, 150));
		frame.setVisible(true);
		System.out.println("<= VTAdminClientImpl.init(" +
			"RemoteObjectProperties " + compProps + " , Object" + s);
	}

	/**
	 * Wird vom Server ausgeführt und nimmt die aktuell verwalteten Clients entgegen,
	 * um sie in der Tabelle auszugeben.
	 */
	public void update(RemoteObjectTable  remoteObjects) throws RemoteException {
		recievedCounter++;
		statuslabel.setText(recievedCounter + " updates recieved");
		System.out.println("\t update recieved: \n" + remoteObjects);
		managedObjects.setDataVector(remoteObjects.getDataVector(), remoteObjects.getKeyVector());
		table.resizeTableColumns();
	}
	
	/**
	 * Initialisiert den ActionListener mit dem Server Objekt, so dass
	 * dieser auf die vote Methode des Servers zugreifen kann.
	 * @param server
	 */
	public void setServerGrap(VTLogicServer server) {
		bv.addActionListener(new VTAdminClientRefreshAction(server, table, statuslabel));
		System.out.println(
			"\t Serverhandle an den Actionlistener übergeben = " + server);
	}

    /* (non-Javadoc)
     * @see projects.interfaces.VTClient#update(projects.voting.model.VoteTable)
     */
    public void update(VoteTable votes) throws RemoteException {
        // TODO Votesupdate verarbeiten bzw. Administration für die Votes implementieren
		System.out.println("=> VTAdminClientImpl.update(VoteTable votes)");
		System.out.println(votes);
		System.out.println("<= VTAdminClientImpl.update(VoteTable votes)");
    }
}
