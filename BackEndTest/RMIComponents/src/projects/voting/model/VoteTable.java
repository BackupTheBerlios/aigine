package projects.voting.model;

import java.io.FileInputStream;
import java.io.IOException;
import java.util.Collections;
import java.util.Enumeration;
import java.util.Hashtable;
import java.util.Properties;
import java.util.Vector;

/**
 * Speichert die Votings und bittet Methoden für das Einlesen
 * und Speichern der Daten in einer Datei.
 * @author danny
 * @since 05.05.2004 21:23:39
 */
public class VoteTable extends Hashtable {
	// TODO Speicherung der Bezeichnung, die vorher noch aus
	// der Datei gelesen werden muss.  DESCRIPTION=description
	// TODO Methode für Speicherung der Datei
	/**
	 * Liest die Votes aus der Datei
	 * Format:
	 * 	voteKey=voteCount|voteDescription
	 * @param filename Name der Datei
	 */
	public VoteTable(String filename) throws IOException {
		super();
		// Datei öffnen und Votes einlesen
		FileInputStream fin = new FileInputStream(filename);
		Properties p = new Properties();
		p.load(fin);
		System.out.println(p);
		// für jedes Vote ein Objekt erzeugen und im HashTable ablegen
		Enumeration pnames = p.propertyNames();
		while (pnames.hasMoreElements()) {
			String pname = (String) pnames.nextElement();
			String svote = p.getProperty(pname);
			Vote vote = new Vote();
			// aufsplitten der Werte
			vote.setCount(
				Integer.parseInt(svote.substring(0, svote.indexOf('|'))));
			vote.setDescription(
				svote.substring(svote.indexOf('|') + 1, svote.length()));
			this.put(pname, vote);
			System.out.println("add vote => id: " + pname + " " + vote);
		}
		System.out.println("complete vote list: \n" + this.toString());
	}

	public String toString() {
		StringBuffer svotes = new StringBuffer();
		Enumeration enum = this.keys();
		while (enum.hasMoreElements()) {
			String key = (String) enum.nextElement();
			Vote vote = (Vote) this.get(key);
			svotes.append(key + " " + vote + "\n");
		}
		return svotes.toString();
	}


	public String toHTML() {
		StringBuffer svotes = new StringBuffer();		
		Enumeration enum = this.keys();
		
		svotes.append("\n<table border='1'>");
		while (enum.hasMoreElements()) {
			svotes.append("\n\t<tr>\n\t\t<td>");
			String key = (String) enum.nextElement();
			Vote vote = (Vote) this.get(key);
			svotes.append(key);
			svotes.append("\n\t\t</td>\n\t\t<td>");
			svotes.append(vote.getCount());
			svotes.append("\n\t\t</td>\n\t\t<td>");
			svotes.append(vote.getDescription());			
			svotes.append("\n\t\t</td>\n\t</tr>");
		}
		svotes.append("\n</table>");
		return svotes.toString();
	}

	public Vector getDataVector() {
		Vector data = new Vector();
		// Sort hashtable.
		Vector keyvector = new Vector(this.keySet());
		Collections.sort(keyvector);
		for (Enumeration e = keyvector.elements(); e.hasMoreElements();) {
			String key = (String) e.nextElement();
			Vote vote = (Vote) this.get(key);
			Vector voteVector = new Vector(); 
			voteVector.add(vote.getDescription());
			voteVector.add(new Integer(vote.getCount()));
			voteVector.add(key);
			data.add(voteVector);
		}
		return data;
	}

	/**
	 * 
	 * @return keys
	 */
	public Vector getKeyVector() {
		Vector keyvector = new Vector();
		keyvector.add("description");
		keyvector.add("count");
		keyvector.add("vote for...");
		return keyvector;
	}
}
