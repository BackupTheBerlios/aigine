/*
 * Created on 08.09.2004
 *
 * Created for RMIComponents project :Q
 */
package API.model;

import java.util.Collection;
import java.util.Set;
import java.util.TreeMap;



/**
 * 
 * 
 * class RemoteObjectTree.java created by @author your mama or at his system
 * created on 08.09.2004 2004 at 03:27:08 
 */
public class RemoteObjectTree extends TreeMap {

	/**
	 * Constructor for RemoteObjectTree object
	 * 
	 * 
	 */
	public RemoteObjectTree() {
		super();
		// TODO Auto-generated constructor stub
	}
	
	public Object put(RemoteObject object, RemoteObjectTable objectTable){
		System.out.println("<=>RemoteObjectTree.put(object:"+object+","+objectTable+")");
		return this.put(object, objectTable);
		
	}
	
	public String toString(){
		String tmp = "Registrierte Services and his single subservices (clients):";
		Collection colect = this.values();
		Object[] colectArray = colect.toArray();
		
		Set set = this.keySet();
		Object[] objects = set.toArray();
		for(int i=0;i < objects.length; i++) {
			RemoteObject rmo = (RemoteObject) objects[i];
			tmp += "\n  Service:"+rmo.compClassName;
			RemoteObjectTable subrmo = (RemoteObjectTable)  colectArray[i];
			Object[] suboj = subrmo.keySet().toArray();
			for(int k = 0; k< suboj.length; k++) {
				String rmot = (String) suboj[k]; 
				
				tmp += "\n\t\t SubServices:"+rmot;
			}
		}
		
		return null;
		
	}

}
