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
 * Dieses Interface erhöht die Typsicherheit für die (RMI-)Kommunikation der 
 * Komponenten mit den DBServer-Komponenten
 */
public class AbstractDTO implements Serializable {
	protected long ID = 0 ;
	
	/**
	 * @return
	 */
	public long getID() {
		return ID;
	}

	/**
	 * @param l
	 */
	public void setID(long l) {
		ID = l;
	}
}
