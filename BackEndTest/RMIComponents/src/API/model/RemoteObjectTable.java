package API.model;

import java.util.Enumeration;
import java.util.Hashtable;
import java.util.Vector;




/**
 * Speichert die RemoteObjects die ein Server anbietet oder verwaltet. Dies
 * können Client, andere Server ... sein
 * @author danny, tobi
 * @since 25.07.2004
 * @version 0.01
 */
public class RemoteObjectTable extends Hashtable {
	
	public RemoteObjectTable() {
		super();
	}

  /**
   * Fügt einen neues RemoteObject hinzu.
   */
	public void add(RemoteObject rop){
		super.put(rop.getApp().toString(), rop);
	}

	/**
   * Gibt einen Vektor mit Informationen zu den angemeldeten RemoteObjects zurück.
   */
	public Vector getDataVector(){
		Enumeration enum = this.elements();
		Vector data = null;
		while(enum.hasMoreElements()) {
			RemoteObject pro = (RemoteObject) enum.nextElement();
			Vector v = new Vector();
			v.add(pro.getRmiName());
			v.add(pro.getCompClassName());
			data.add(v);			
		}
		return data;
	}

	/**
   * Gibt einen Vektor mit den Schlüsselwerten für den <code>dataVector</code> zurück.
   */
	public Vector getKeyVector(){
		Vector v = new Vector();
		v.add("RMI Name");
		v.add("Comp ClassName");		
		return v;
	}
}
