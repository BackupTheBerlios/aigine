/*
 * Created on 07.05.2004
 *
 */
package projects.voting.model;

import java.util.Enumeration;
import java.util.Vector;

import javax.swing.ButtonGroup;
import javax.swing.JRadioButton;
import javax.swing.table.DefaultTableModel;

/**
 * Erweitertes <code>DefaultTableModel</code> mit einer zusätzlichen
 * Spalte die eine <code> ButtonGroup</code> für die Auswahl der
 * Votings enthält.
 * @author danny
 * @since 07.05.2004 15:17:40
 */
public class VTClientTableModel extends DefaultTableModel {
	
	private ButtonGroup buttonGroup;
	/**
	 * @param data
	 * @param columnNames
	 */
	public VTClientTableModel(VoteTable votes) {
		super(votes.getDataVector(), votes.getKeyVector());
		addButtonGroupColumn(votes.getDataVector());
	}
	
	/**
	 * 
	 */
	public VTClientTableModel() {
		super();
	}	
	
	/**
	 * Setzt die Daten erneut.
	 * @see javax.swing.table.DefaultTableModel#setDataVector(java.lang.Object[][], java.lang.Object[])
	 */
	public void setDataVector(Vector dataVector, Vector columnIdentifiers) {
		super.setDataVector(dataVector, columnIdentifiers);
		addButtonGroupColumn(dataVector);
	}		
	
	/**
	 *  Fügt eine Spalte mit den RadioButtons hinzu
	 *  @param dataVector
	 */
	private void addButtonGroupColumn(Vector dataVector){
		buttonGroup = new ButtonGroup();
		for (Enumeration e = dataVector.elements(); e.hasMoreElements();) {
			Vector c = (Vector) e.nextElement();
			JRadioButton b = new JRadioButton((String)c.get(2));
			b.setActionCommand((String)c.get(2));
			c.set(2, b);
			buttonGroup.add(b);
		}
	}
	
	/**
	 * @return buttonGroup
	 */
	public ButtonGroup getButtonGroup() {
		return buttonGroup;
	}

}
