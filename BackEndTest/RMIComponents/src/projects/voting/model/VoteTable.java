package projects.voting.model;

import java.util.Collections;
import java.util.Enumeration;
import java.util.Hashtable;
import java.util.Vector;

/**
 * Speichert die Votings
 * @author danny, franky
 * @since 05.05.2004 21:23:39
 */
public class VoteTable extends Hashtable {

    public VoteTable() {
        super();
    }

    public String toString() {
        StringBuffer svotes= new StringBuffer();
        Enumeration enum= this.keys();
        while (enum.hasMoreElements()) {
            String key= (String) enum.nextElement();
            Vote vote= (Vote) this.get(key);
            svotes.append(key + " " + vote + "\n");
        }
        return svotes.toString();
    }

    public Vector getDataVector() {
        Vector data= new Vector();
        // Sort hashtable.
        Vector keyvector= new Vector(this.keySet());
        Collections.sort(keyvector);
        for (Enumeration e= keyvector.elements(); e.hasMoreElements();) {
            String key= (String) e.nextElement();
            Vote vote= (Vote) this.get(key);
            Vector voteVector= new Vector();
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
        Vector keyvector= new Vector();
        keyvector.add("description");
        keyvector.add("count");
        keyvector.add("vote for...");
        return keyvector;
    }

    public String toHTML() {
        StringBuffer svotes= new StringBuffer();
        Enumeration enum= this.keys();
        svotes.append("\n<table border='1'>");
        while (enum.hasMoreElements()) {
            svotes.append("\n\t<tr>\n\t\t<td>");
            String key= (String) enum.nextElement();
            Vote vote= (Vote) this.get(key);
            svotes.append(vote.getDescription());
            svotes.append("\n\t\t</td>\n\t\t<td>");
            svotes.append(vote.getCount());
            svotes.append("\n\t\t</td>\n\t\t<td>");
            svotes.append(
                "<input type='radio' name='voteradio' value='"
                    + key
                    + "'>"
                    + key);
            svotes.append("\n\t\t</td>\n\t</tr>");
        }
        svotes.append("\n</table>");
        return svotes.toString();
    }
}
