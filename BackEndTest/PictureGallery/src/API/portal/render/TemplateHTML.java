/*
 * Created on 03.01.2005
 *
 * @user drichter
 * */
package API.portal.render;

import java.util.Hashtable;

/**
 * @author drichter
 * @since 2005-01-03
 * 
 */
public class TemplateHTML {
	private String head = null ;
	private String foot = null ;
	private Hashtable frameStarts = new Hashtable() ;
	private Hashtable frameEnds = new Hashtable() ;
	private Hashtable blockStarts = new Hashtable() ;
	private Hashtable blockEnds = new Hashtable() ;
	
	public void initTemplateFromFile(String filename) {
		// LATER hier können Dateien eingelesen werden für Templatevarianten
		
	}
	
	/**
	 * Description: internal Method to grep an HTML-Code-Element from one of the Hashtables
	 * @author drichter
	 * @since 2005-01-03
	 * 
	 * */
	
	private String getPart(int nr, Hashtable ht) {
		String thePart = new String("") ;
		Integer Nummer = new Integer(nr) ;
		if (ht.containsKey(Nummer)) {
			thePart = (String) ht.get(Nummer) ; 
		}
		return thePart ;
	}
	
	/**
	 * @return den gewünschten BlockStart-Eintrag dieses Templates
	 */
	public String getBlockStart(int nr) {
		return getPart(nr, blockStarts) ;
	}

	/**
	 * @return den gewünschten BlockEnd-Eintrag dieses Templates
	 */
	public String getBlockEnd(int nr) {
		return getPart(nr, blockEnds) ;
	}

	/**
	 * @return den gewünschten FrameStart-Eintrag dieses Templates
	 */
	public String getFrameStart(int nr) {
		return getPart(nr, frameStarts) ;
	}

	/**
	 * @return den gewünschten FrameEnd-Eintrag dieses Templates
	 */
	public String getFrameEnd(int nr) {
		return getPart(nr, frameEnds) ;
	}

	/**
	 * @return Den Kopf dieses Templates
	 */
	public String getHead() {
		return head;
	}

	/**
	 * @return Den Fuss dieses Templates
	 */
	public String getFoot() {
		return foot;
	}

	/** 
	 * Description: fügt einen BlockStart hinzu, die Nummer wird dabei chronologisch vergeben
	 * @author drichter
	 * @since 2005-01-03
	 * 
	 * */
	public void addBlockStart(String thePart) {
		blockStarts.put(new Integer(blockStarts.size() + 1), thePart) ;
	}

	/** 
	 * Description: fügt einen BlockEnd hinzu, die Nummer wird dabei chronologisch vergeben
	 * @author drichter
	 * @since 2005-01-03
	 * 
	 * */
	public void addBlockEnd(String thePart) {
		blockEnds.put(new Integer(blockEnds.size() + 1), thePart) ;
	}

	/** 
	 * Description: fügt einen FrameStart hinzu, die Nummer wird dabei chronologisch vergeben
	 * @author drichter
	 * @since 2005-01-03
	 * 
	 * */
	public void addFrameStart(String thePart) {
		frameStarts.put(new Integer(frameStarts.size() + 1), thePart) ;
	}

	/** 
	 * Description: fügt einen FrameEnd hinzu, die Nummer wird dabei chronologisch vergeben
	 * @author drichter
	 * @since 2005-01-03
	 * 
	 * */
	public void addFrameEnd(String thePart) {
		frameEnds.put(new Integer(frameEnds.size() + 1), thePart) ;
	}


	/**
	 * @param hashtable
	 */
	public void setBlockStarts(Hashtable hashtable) {
		blockStarts = hashtable;
	}

	/**
	 * @param hashtable
	 */
	public void setFrameEnds(Hashtable hashtable) {
		frameEnds = hashtable;
	}

	/**
	 * @param hashtable
	 */
	public void setFrameStarts(Hashtable hashtable) {
		frameStarts = hashtable;
	}

	/**
	 * @param string
	 */
	public void setHead(String string) {
		head = string;
	}

	/**
	 * @param string
	 */
	public void setFoot(String string) {
		foot = string;
	}



}