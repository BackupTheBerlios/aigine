/*
 * Created on 10.09.2004
 *
 * Created for PictureGallery project :Q
 */
package projects.catalog;

import java.rmi.RemoteException;
import java.util.Hashtable;

import projects.interfaces.CProjectServer;
import API.interfaces.ManagerHandle;

import API.control.Server ;
import API.model.RemoteObject;
import API.model.RemoteObjectTable;
import API.portal.model.Block;
import API.portal.model.BlockContent;

/**
 * 
 * 
 * class CProjectServerImpl.java created by @author your mama or at his system
 * created on 10.09.2004 2004 at 14:52:37 
 */
public class CProjectServerImpl extends Server implements CProjectServer {
	private ManagerHandle manager;
	private String WebRequestError = null;
	
	/**
	 * Constructor for CProjectServerImpl object
	 * 
	 * 
	 */
	public CProjectServerImpl() throws RemoteException {
		super();
		// TODO Auto-generated constructor stub
	}

	/**
	 * description:
	 * 
	 * @param service
	 * @return
	 * @throws RemoteException 
	 * @see API.interfaces.ServerHandle#register(API.model.RemoteObject)
	 */
	public String register(RemoteObject service) throws RemoteException {
		// TODO Auto-generated method stub
		return null;
	}

	/**
	 * description:
	 * 
	 * @param remoteObject
	 * @param usr
	 * @param pass
	 * @return
	 * @throws RemoteException 
	 * @see API.interfaces.ServerHandle#register(API.model.RemoteObject, java.lang.String, java.lang.String)
	 */
	public String register(RemoteObject remoteObject, String usr, String pass)
			throws RemoteException {
		// TODO Auto-generated method stub
		return null;
	}

	/**
	 * description:
	 * 
	 * @param remoteObject
	 * @param usr
	 * @param pass
	 * @return
	 * @throws RemoteException 
	 * @see API.interfaces.ServerHandle#registerAdminClient(API.model.RemoteObject, java.lang.String, java.lang.String)
	 */
	public String registerAdminClient(RemoteObject remoteObject, String usr,
			String pass) throws RemoteException {
		// TODO Auto-generated method stub
		return null;
	}

	/**
	 * description:
	 * 
	 * @param remoteObject
	 * @return
	 * @throws RemoteException 
	 * @see API.interfaces.ServerHandle#registerService(API.model.RemoteObject)
	 */
	public String registerService(RemoteObject remoteObject)
			throws RemoteException {
		// TODO Auto-generated method stub
		return null;
	}

	/**
	 * description:
	 * 
	 * @return
	 * @throws RemoteException 
	 * @see API.interfaces.ServerHandle#checkClientConnections()
	 */
	public RemoteObjectTable checkClientConnections() throws RemoteException {
		// TODO Auto-generated method stub
		return null;
	}

	/**
	 * description:
	 * 
	 * @return 
	 * @see projects.interfaces.CLoginServer#getManager()
	 */
	public ManagerHandle getManager() {
		return manager;
	}

	/**
	 * description:
	 * 
	 * @param manager 
	 * @see projects.interfaces.CLoginServer#setManager(projects.interfaces.CManagerServer)
	 */
//	public void setManager(ManagerHandle localmanager) {
//		this.manager = localmanager ;
//	}
	
	/**
	 * description:
	 * 
	 * @param manager 
	 */
	private Block showimage(Hashtable requests) {
		System.out.println() ;
		return null ;
	}
	
	/**
	 * description:
	 * 
	 * @param Requests as Hashtable
	 */
	private Block showimages(Hashtable requests) {
		Block result = null ;
		System.out.println("==> projects.catalog.CProjectServerImpl.showimages") ;
		
		System.out.println("  > folgende Requests sind angekommen") ;
		
		System.out.println("<== projects.catalog.CProjectServerImpl.showimages") ;
		BlockContent bc = new BlockContent("Testbild", "image") ;
		bc.addAttribute("src", "img/pferde/img_1.jpg") ;
		bc.addAttribute("alt", "Bild eines Pferdes") ;
		bc.addAttribute("border", "3") ;
		result = new Block(bc) ;
		result.setTitle("ich zeige Bilder:") ;
		return result ;
	}
	
	/**
	* verarbeitet einen 'WebRequest' der momentan vom Webserver an beliebige andere Server geschickt werden kann
	* 
	* @author Dennis
	* @since 16.09.2004
	* @param Operation, welcher Block, Parameterpaare und evtl. später mit übertragenem Body
	* @return Block (serializable)
	* @throws RemoteException
	*/
	public Block executeWebRequest(String op, String whichBlock, Hashtable requestProps) throws RemoteException {
		Block result = null ;
		WebRequestError = null ;
		System.out.println("\n\n>>> habe einen WebRequest erhalten: " + op) ;
		if (op.indexOf("serverinfo") == 0) {
			result = new Block(this.toString()) ;
			result.setTitle("Serverinformation") ;
		} else if (op.indexOf("userinfo") == 0) {
			result = new Block("hier erscheinen dann wohl Userdaten, vllt aber auch vom LoginServer") ;
			result.setTitle("Userinfos") ;
			result.setStyle("copyright") ;
		} else if (op.indexOf("seitenkopf") == 0) {
			result = new Block("Auch dieses wird dynamisch vom CProjectServer generiert und kann daher flexibel eingesetzt werden!") ;
			result.setTitle("Kopf des Portals") ;
		} else if (op.indexOf("kategorien") == 0) {
			result = new Block("Und dies ist die Kategoriennavigation ...") ;
			result.setTitle("Kategorien") ;
		} else if (op.indexOf("toplist") == 0) {
			result = new Block("Mit DB-Hilfe erscheint hier dann die Topliste!") ;
			result.setTitle("Toplist Bilder!!") ;
		} else if (op.indexOf("main") == 0) {
			BlockContent bc = new BlockContent("Hier soll dann die Musik spielen!") ;
			BlockContent tempbc = new BlockContent("Und das dann auch noch in mehreren Elementen (hier: Absätzen), wie hier demonstriert! Wobei dieser hier hervorgehoben wird.") ;
			BlockContent bcEnde = null ;
			BlockContent bcSubEnde1 = null ;
			tempbc.addAttribute("class","wichtig") ;
			bc.setNachfolger(tempbc) ;
			bcEnde = tempbc ;
			tempbc = new BlockContent("Und weil es so schön ist, hier noch ein Absatz ... mit einer anschließenden ", "text") ;
			bcEnde.setNachfolger(tempbc) ;
			bcEnde = tempbc ;
			tempbc = new BlockContent("Linkliste:", "ulist") ;
			bcEnde.setNachfolger(tempbc) ;
			bcEnde = tempbc ;
			
			tempbc = new BlockContent("1. Link:", "listpoint") ;
			bcEnde.setSubContent(tempbc) ;
			bcSubEnde1 = tempbc ;
			
			tempbc = new BlockContent("rechten Block von Kategorien auf Toplist ändern", "link") ;
			tempbc.addAttribute("href","catalog.html?srv=CProjectServer&block=kategorien&op=toplist") ;
			tempbc.addAttribute("class","wichtig") ;
			//tempbc.addAttribute("class","wichtig") ;
			bcSubEnde1.setSubContent(tempbc) ;
			
			tempbc = new BlockContent("1. Link:", "listpoint") ;
			bcSubEnde1.setNachfolger(tempbc) ;
			bcSubEnde1 = tempbc ;
			
			tempbc = new BlockContent("rechten Block von Toplist auf Kategorien ändern", "link") ;
			tempbc.addAttribute("href","catalog.html?srv=CProjectServer&block=kategorien&op=kategorien") ;
			tempbc.addAttribute("class","wichtig") ;
			tempbc.addAttribute("class","wichtig") ;
			bcSubEnde1.setSubContent(tempbc) ;

			
//			bcEnde = tempbc ;
			
			result = new Block(bc) ;
			result.setTitle("Hauptbereich") ;
			result.setStyle("main") ;
			
	//		<a href=\"catalog.html?block=kategorien&srv=CProjectServer&op=toplist\">test</a>
		} else if (op.indexOf("showimages") == 0) {
			result = showimages(requestProps) ;
		} else { 
			WebRequestError = new String("unexpected Operation") ;
			result = new Block("unexpected Block-Operation") ;
			result.setTitle("internal Error") ;
		}
		System.out.println("\n\n  > ich antworte mit: " + result.getTitle()) ;
		return result ;
	}

}
