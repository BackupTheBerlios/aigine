/*
 * Created on 17.03.2005
 *
 * @user drichter
 * */
package API.model;

import java.io.Serializable;

/**
 * @author drichter
 *
 * Dieses Interface erh�ht die Typsicherheit f�r die (RMI-)Kommunikation der 
 * Komponenten mit den DBServer-Komponenten
 */
public class AbstractDTO implements Serializable {
	protected String ID = null ;
	
	/**
	 * @return
	 */
	public String getID() {
		return ID;
	}

	/**
	 * @param l
	 */
	public void setID(String theID) {
		ID = theID;
	}
}
