package projects.voting;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.rmi.RemoteException;

import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JScrollPane;

import projects.interfaces.VTClient;
import projects.interfaces.VTServer;
import projects.voting.control.JComponentCellEditor;
import projects.voting.control.JComponentCellRenderer;
import projects.voting.control.JVoteTable;
import projects.voting.control.VTClientVoteAction;
import projects.voting.model.VTClientTableModel;
import projects.voting.model.VoteTable;
import API.interfaces.ServerHandle;
import API.model.RemoteObject;

/**
 * 
 * @author danny
 * @since 05.05.2004 20:16:24
 */
public class VTClientImpl implements VTClient {

	private JFrame frame = new JFrame();
	private JLabel statuslabel = new JLabel("tschesch kollega");
	private JVoteTable votetable = new JVoteTable();
	private JButton bv = new JButton("vote");
	private VTClientTableModel vtmodel = new VTClientTableModel();

	private int recievedCounter = 0;


	public void init(RemoteObject compProps, ServerHandle server) {
		System.out.println("=> VTClientImpl.init(" +			"RemoteObjectProperties " + compProps + " , Object" + server);
		VTServer s = (VTServer) server;
		
		System.out.println(
			"\tVTClientImpl.init() service inhalt: " + compProps);
		frame.setTitle(compProps.getCompName());
		setServerGrap(s);
		System.out.println(
			"\t Client bei Server regestriert "
				+ "---------------------Allet schick!!!! HELDEN!!!");
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.getContentPane().add(statuslabel, BorderLayout.SOUTH);
		
		// Tabelleninitialisierung
		JScrollPane tablepanel = new JScrollPane();
		votetable.setModel(vtmodel);
		votetable.setDoubleBuffered(true);
		votetable.setDefaultRenderer(
			JComponent.class,
			new JComponentCellRenderer());
		votetable.setDefaultEditor(
			JComponent.class,
			new JComponentCellEditor());
		tablepanel.setColumnHeaderView(votetable.getTableHeader());
		tablepanel.setViewportView(votetable);
		frame.getContentPane().add(tablepanel);
		frame.getContentPane().add(bv, BorderLayout.EAST);
		//Display the window.
		frame.setSize(new Dimension(500, 150));
		frame.setVisible(true);
		System.out.println("<= VTClientImpl.init(" +
			"RemoteObjectProperties " + compProps + " , Object" + s);
	}

	/* (non-Javadoc)
	 * @see projects.projects.Client.VoteUpdate#update(java.lang.String)
	 */
	public void update(VoteTable votes) throws RemoteException {
		// TODO update der Tabelledaten verbessern, es sollte immer nur
		// das aktuell geänderte Voting übertragen werden
		// => Aufsplittung in zwei Funktionen: 
		// 1. setzen der gesamten Tabelle
		// 2. update eines Vote Eintrages 
		recievedCounter++;
		statuslabel.setText(recievedCounter + " updates recieved");
		System.out.println("\tupdate recieved: \n" + votes);
		vtmodel.setDataVector(votes.getDataVector(), votes.getKeyVector());
		votetable.resizeTableColumns();
	}

	/**
	 * Initialisiert den ActionListener mit dem Server Objekt, so dass
	 * dieser auf die vote Methode des Servers zugreifen kann.
	 * @param server
	 */
	public void setServerGrap(VTServer server) {
		bv.addActionListener(new VTClientVoteAction(server, vtmodel));
		System.out.println(
			"\t Serverhandle an den Actionlistener übergeben = " + server);
	}
}
