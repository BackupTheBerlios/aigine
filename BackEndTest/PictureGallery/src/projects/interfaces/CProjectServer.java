/*
 * Created on 10.09.2004
 *
 * Created for PictureGallery project :Q
 */
package projects.interfaces;

import API.interfaces.ServerHandle;

/**
 * CProjectServer ist der eigentliche Logic-/ bez. View-Server. Er kuemmert sich um
 * die web-bezogenen Anfragen bezueglich Bilder Cataloge
 * Generiert projectbezogene html_struckturen (blocks)
 * 
 * 
 * class CProjectServer.java created by @author your mama or at his system
 * created on 10.09.2004 2004 at 14:50:20 
 */
public interface CProjectServer extends ServerHandle {
	CManagerServer manager = null;

	void setManager(CManagerServer manager)throws java.rmi.RemoteException ;
}
