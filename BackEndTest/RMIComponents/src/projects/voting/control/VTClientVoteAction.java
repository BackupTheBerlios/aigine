package projects.voting.control;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import projects.interfaces.VTLogicServer;
import projects.voting.model.VTClientTableModel;


/**
 * ActionListener für die Abgabe eines Votes.
 * @author danny
 * @since 07.05.2004 14:17:18
 */
public class VTClientVoteAction implements ActionListener {

	private VTLogicServer server = null;
	private VTClientTableModel votetable = null;

	/**
	 *  Konstruktor
	 * @param server Objekt für den Serverzugriff.
	 * @param votetable TableModel mit der ButtonGroup für
	 * die Votingausgabe.
	 */
	public VTClientVoteAction(VTLogicServer server, VTClientTableModel votetable) {
		super();
		this.server = server;
		this.votetable = votetable;
	}

	/**
	 * Setzt ein Voting für den aktuell gewählten RadioButton auf
	 * dem Server
	 * @param ActionEvent
	 */
	public void actionPerformed(ActionEvent ae) {
		try {
			String vote = votetable.getButtonGroup().getSelection().getActionCommand();
			server.vote(vote);
		} catch ( Exception e ) {
			e.printStackTrace();
			System.out.println("Fehler beim Voting!");
		}
	}
}
