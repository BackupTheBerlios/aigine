package projects.voting.control;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import projects.interfaces.VTServer;


/**
 * ActionListener für die Abgabe eines Votes.
 * @author danny
 * @since 07.05.2004 14:17:18
 */
public class VTAdminClientAction implements ActionListener {

	private VTServer server = null;

	/**
	 *  Konstruktor
	 * @param server Objekt für den Serverzugriff.
	 */
	public VTAdminClientAction(VTServer server) {
		super();
		this.server = server;
	}

	/**
	 * @param ActionEvent
	 */
	public void actionPerformed(ActionEvent ae) {
		try {
			// TODO Beim Server ein update der Daten anfordern.			
			//server.vote(vote);
		} catch ( Exception e ) {
			e.printStackTrace();
			System.out.println("Fehler beim Voting!");
		}
	}
}
