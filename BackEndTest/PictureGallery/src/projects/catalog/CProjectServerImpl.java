/*
 * Created on 10.09.2004
 *
 * Created for PictureGallery project :Q
 */
package projects.catalog;

import java.rmi.RemoteException;
import java.util.Hashtable;
import java.util.Properties;

import projects.catalog.model.PictureDTO;
import projects.interfaces.CDbServerHandle;
import projects.interfaces.CProjectServer;
 // import API.interfaces.DbServerHandle;
import API.interfaces.ManagerHandle;
// import API.interfaces.ServerHandle;

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
	
	private BlockContent PictureToBlockContent(PictureDTO thePic, int height, int width) {
		BlockContent bcRoot = null ;
		
		bcRoot = new BlockContent("(Klick)","link") ;
		bcRoot.addAttribute("href", "catalog.html?srv=CProjectServer&block=main&op=showimage&imgid=" + thePic.getID()) ;
		
		BlockContent bc = new BlockContent(thePic.getTitel(),"image") ;
		bc.addAttribute("src", "leerer Pfad zur Zeit") ; // thePic.getPath(), geht nicht, die Pfade stehen im Array darunter drin
		bc.addAttribute("alt", thePic.getTitel()) ;
		bc.addAttribute("border", "1") ;
		if (height >= 0) {
			bc.addAttribute("height", new Integer(height).toString()) ;
			bc.addAttribute("width", new Integer(width).toString()) ;	
		}
		bcRoot.setNachfolger(bc) ;
		
		BlockContent bctemp = new BlockContent("Dieses Bild \"" + thePic.getTitel() + "\" hat das Format derzeit unklar","text") ; // steht erst zur jeweiligen Datei dabei, thePic.getFormat()
		bc.setNachfolger(bctemp) ;
		// bc = bctemp ;
		
		bcRoot.setNachfolger(bc) ;
		return bcRoot ;
	}
	
	
	
	/**
	 * description:
	 * 
	 * @param manager 
	 */
	private Block showimage(Hashtable requests) {
		System.out.println("==> projects.catalog.CProjectServerImpl.showimage") ;
		Block result = null ;
		PictureDTO thePic = null ;
		BlockContent bcRoot = null;
		BlockContent bc = null ;


		System.out.println("==> projects.catalog.CProjectServerImpl.showimages") ;

		CDbServerHandle dbserver = (CDbServerHandle) this.getServer("CDBServer") ;

		if (dbserver != null) {
			try {
				System.out.println("  > Diesen Wert senden wir an den CDBServer: " + requests.get("imgid").toString()) ;
				// thePic = dbserver.getPicture(requests.get("imgid").toString()) ;
				Properties param = new Properties() ;
				param.put("ID_equal", requests.get("imgid").toString()) ;
				thePic = (PictureDTO) dbserver.load(CDbServerHandle.PICTURE, param) ;
			} catch (RemoteException re) {
				System.out.println("--- Es ist eine RemoteException beim Aufruf von CDBServerImpl.getPictures() aufgetreten: " + re) ;
			
			}
		} else {
			System.out.println("--- CDBServer ist nicht erreichbar ...") ;
			bcRoot = new BlockContent("Unser Datenbankserver ist leider im Moment nicht erreichbar. Bitte versuchen Sie es später. Our Database is unreachable. Please try later.") ;
		}
		if (thePic != null) {
			bcRoot = new BlockContent("Das von Ihnen gewünschte Bild:","text") ;
//			bc = bcRoot ;
			BlockContent bctemp = new BlockContent("Bild Nr. " + thePic.getID(),"text") ; 
			bctemp.setSubContent(PictureToBlockContent(thePic, -1, -1)) ; // TODO muss um Schalter erweitert werden, ob Thumb oder Vollbild, welche Attribute usw.
			bcRoot.setNachfolger(bctemp) ;
//			bc = bctemp ;
		} else {
			System.out.println("Keine Bilder mit diesen Kriterien in der Datenbank") ;
			bcRoot = new BlockContent("Unter diesen Kriterien gibt es derzeit leider keine Bilder in unserer Datenbank ...", "text") ;
			bcRoot.addAttribute("class", "fehler") ;
		}
		result = new Block(bcRoot) ;
		result.setTitle("Bild '" + thePic.getTitel() + "'") ;

		System.out.println("<== projects.catalog.CProjectServerImpl.showimage") ;

		return result ;
	}
	
	
	
	/**
	 * description:
	 * 
	 * @param Requests as Hashtable
	 */
	private Block showimages(Hashtable requests) {
		Block result = null ;
		PictureDTO[] pictures = null ;
		BlockContent bcRoot = null;
		BlockContent bc = null ;
		
		
		System.out.println("==> projects.catalog.CProjectServerImpl.showimages") ;
		
		CDbServerHandle dbserver = (CDbServerHandle) this.getServer("CDBServer") ;
		
		if (dbserver != null) {
			try {
				Properties props = new Properties() ;
				
				pictures = (PictureDTO[]) dbserver.loadList(CDbServerHandle.PICTURE, props) ;
				//dbresult = dbserver.getPictures("Pferde", 1, 0) ;
			} catch (RemoteException re) {
				System.out.println("--- Es ist eine RemoteException beim Aufruf von CDBServerImpl.getPictures() aufgetreten: " + re) ;
			}
		} else {
			System.out.println("--- CDBServer ist nicht erreichbar.") ;
			bcRoot = new BlockContent("Unser Datenbankserver ist leider im Moment nicht erreichbar. Bitte versuchen Sie es später. Our Database is unreachable. Please try later.") ;
		}
		if (pictures != null) {
			if (pictures.length == 0) {
				System.out.println("Keine Bilder mit diesen Kriterien in der Datenbank") ;
				bcRoot = new BlockContent("Unter diesen Kriterien gibt es derzeit leider keine Bilder in unserer Datenbank ...", "text") ;
				bcRoot.addAttribute("class", "fehler") ;
			}
			int zaehler = 1 ;
			bcRoot = new BlockContent("Diese Bilder sind derzeit in unserer Datenbank eingetragen:","text") ;
			bc = bcRoot ;
			System.out.println("  > Anzahl der Bilder: " + pictures.length) ;
			//while (dbresult.containsKey(new Integer(zaehler))) {
			for (int i = 0; i < pictures.length; i++) {
				BlockContent bctemp = new BlockContent("Bild Nr. " + zaehler,"text") ; 
				bctemp.setSubContent(PictureToBlockContent(pictures[i], 50, 50)) ; // TODO muss um Schalter erweitert werden, ob Thumb oder Vollbild, welche Attribute usw.
				bc.setNachfolger(bctemp) ;
				bc = bctemp ;
				zaehler++ ;
			}
	/*		BlockContent bc = new BlockContent("Testbild", "image") ;
			bc.addAttribute("src", "img/pferde/img_1.jpg") ;
			bc.addAttribute("alt", "Bild eines Pferdes") ;
			bc.addAttribute("border", "3") ;*/
			
		} else {
			System.out.println("--- Rückgabe von CDBServer ist null.") ;
			bcRoot = new BlockContent("Interner Serverfehler.") ;
		}
		result = new Block(bcRoot) ;
		result.setTitle("Bilder der PictureGallery") ;
		
		System.out.println("<== projects.catalog.CProjectServerImpl.showimages") ;
		
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
//		System.out.println("  > die Hashtable mit den Request enthält folgende Daten: " + requestProps) ;
		if (op.indexOf("serverinfo") == 0) {
			result = new Block(this.toString()) ;
			result.setTitle("Serverinformation") ;
		} else if (op.indexOf("userinfo") == 0) {
			result = new Block("hier erscheinen dann wohl Userdaten, vllt aber auch vom LoginServer") ;
			result.setTitle("Userinfos") ;
			result.setStyle("copyright") ;
		} else if (op.indexOf("seitenkopf") == 0) {
			BlockContent bc = new BlockContent("Dieser Block wird dynamisch vom CProjectServer generiert und kann daher flexibel eingesetzt werden.","text") ;
			BlockContent tempbc = new BlockContent(" Link zur Startseite ","link") ;
			tempbc.addAttribute("href","catalog.html?srv=CProjectserver") ;
			bc.setSubContent(tempbc) ;
			BlockContent tempbc2 = new BlockContent("Absatz nach dem Link","text") ;
			tempbc.setNachfolger(tempbc2) ;
			result = new Block(bc) ;
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
			
		} else if (op.indexOf("showimages") == 0) {
			result = showimages(requestProps) ;
		} else if (op.indexOf("showimage") == 0) {
			result = showimage(requestProps) ;
		} else { 
			result = new Block("unexpected Block-Operation") ;
			result.setTitle("internal Error") ;
		}
		System.out.println("\n\n  > ich antworte mit: " + result.getTitle()) ;
		return result ;
	}

}
