/*
 * Created on 14.03.2005
 *
 * @user drichter
 * */
package API.interfaces;

import java.rmi.RemoteException;
import java.util.ArrayList;
import java.util.Properties;

import API.model.AbstractDTO;

//import API.portal.model.RequestFrames;

/**
 * Dieses Interface definiert die allgemeinen Methoden, die der DbServer zur Verfügung stellt.
 * Die Interfaces für die konkreten DBServer-Implementierungen des Portals oder der Projekte sollten wenn notwendig davon abgeleitet werden.
 * 
 * @author drichter
 */
public interface DbServerHandle extends ServerHandle {
	
	/**
	 * 
	 * @author drichter
	 *
	 * 
	 */
	
	public int insert(int whichDAO, AbstractDTO theDTO) throws RemoteException ;
	
	public int update(int whichDAO, AbstractDTO theDTO, Properties parameter) throws RemoteException ;
	
	public AbstractDTO[] loadList(int whichDAO, Properties parameter) throws RemoteException ;
	
	public AbstractDTO load(int whichDAO, Properties parameter) throws RemoteException ;
	
	public int delete(int whichDAO, Properties parameter) throws RemoteException ;
	
	
/*	public void saveFrameSet(RequestFrames rfs) throws RemoteException ;
	
	public RequestFrames getFrameSet(String template, String theSession) throws RemoteException ; */

}
