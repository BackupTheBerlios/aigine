package projects.interfaces;

import projects.catalog.control.CSession;
import API.interfaces.ServerHandle;





/**
 * @author your mama
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public interface CLoginServer extends ServerHandle {
	
	CSession createSession(Long usrID);
}