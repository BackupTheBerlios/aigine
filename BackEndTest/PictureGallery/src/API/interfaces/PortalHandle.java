/*
 * Created on 07.11.2004
 *
 * To change the template for this generated file go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
package API.interfaces;
import java.rmi.*;
import java.util.Hashtable;


/**
 * @author Dennis
 *
 * To change the template for this generated type comment go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
public interface PortalHandle extends ServerHandle {

	public String getPortalAsHTML(Hashtable requestProps) throws RemoteException ;
}
