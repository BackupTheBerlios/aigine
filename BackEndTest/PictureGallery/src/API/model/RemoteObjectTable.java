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
		System.out.println("=> RemoteObjectTable.add(RemoteObject " + rop + ")");
		System.out.println("\t KEY =  " + rop.getApp().toString());
		super.put(rop.getApp().toString(), rop);
		System.out.println("<= RemoteObjectTable.add(RemoteObject " + rop + ")");
	}

	/**
	  * Fügt einen neues RemoteObject hinzu.
	  */
	   public void add(RemoteObject rop, String manager){
		   System.out.println("=> RemoteObjectTable.add(RemoteObject " + rop + ")");
		   super.put(rop.getRmiName() + manager, rop);
		   System.out.println("<= RemoteObjectTable.add(RemoteObject " + rop + ")");
	   }	
	

	/**
   * Gibt einen Vektor mit Informationen zu den angemeldeten RemoteObjects zurück.
   */
	public Vector getDataVector(){
		System.out.println("=> RemoteObjectTable.getDataVector()");
		Enumeration enum = this.elements();
		Vector data = new Vector();
		while(enum.hasMoreElements()) {
			RemoteObject pro = (RemoteObject) enum.nextElement();
			Vector v = new Vector();
			//v.add(pro.getRmiName());
			//v.add(pro.getCompClassName());	
			v.add(pro.getApp());			
			data.add(v);
			System.out.println("\t" + pro.getRmiName() + "\t|" + pro.getCompClassName());
		}
		System.out.println("<= RemoteObjectTable.getDataVector()");
		return data;
	}

	/**
   * Gibt einen Vektor mit den Schlüsselwerten für den <code>dataVector</code> zurück.
   */
	public Vector getKeyVector(){
		Vector v = new Vector();
		//v.add("RMI Name");
		//v.add("Comp ClassName");
		v.add("Object");			
		return v;
	}
    /**
     * nimmt ein RemoteObject entgegen und löscht es 
     * aus dem HashTable . key -> siehe <ode>add()</code> 
     * @see java.util.Dictionary#remove(java.lang.Object)
     */
    public synchronized Object remove(Object remoteObject) {
		System.out.println("RemoteObjectTable.remove(" + remoteObject + ")");
		RemoteObject rop = (RemoteObject)remoteObject;
		System.out.println("\t KEY =  " + rop.getApp().toString());		
        return super.remove(rop.getApp().toString());
    }
	
	/**
	 * nimmt ein RemoteObject entgegen und löscht es 
	 * aus dem HashTable . key -> siehe <ode>add()</code> 
	 * @see java.util.Dictionary#remove(java.lang.Object)
	 */
	public synchronized Object remove(Object remoteObject, String manager) {
		System.out.println("RemoteObjectTable.remove(" + remoteObject + ")");
		RemoteObject rop = (RemoteObject)remoteObject;
		return super.remove(rop.getRmiName() + manager);
	}
}
