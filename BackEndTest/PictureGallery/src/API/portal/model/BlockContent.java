/*
 * Created on 16.09.2004
 *
 * 
 */
package API.portal.model;

import java.util.Enumeration;
import java.util.Hashtable;

import API.model.AbstractDTO;

/**
 * Description: Diese Klasse dient dem Aufbau hierarchischer Block-Strukturen, jeder Knoten
 * kann Unterknoten enthalten
 * @author Dennis
 * @since 2004-09-16
 * 
 */
public class BlockContent extends AbstractDTO {
// TODO (DR) tree aus Java-Standard-API nutzen?
	private BlockContent nachfolger = null ;
	private BlockContent subContent = null ;
	// subContent enthält eine untergeordnete Kette, dies erlaubt Tabellen, Links u.ä.
	private String content = new String("leerer Content-Knoten") ;
	private String typ = null ;
	private Hashtable attributes = new Hashtable() ;
		
	public BlockContent (String thecontent) {
		init(thecontent, "text") ;
		// Question @Danny: wie kann man verschiedene Konstruktoren sich gegenseitig aufrufen lassen?
	}
	
	public BlockContent (String thecontent, String thetyp) {
		init(thecontent, thetyp) ;
	}
	
	private void init (String thecontent, String thetyp) {
		this.content = thecontent ;
		this.typ = thetyp ;
	}
	
	public void setNachfolger(BlockContent theNachfolger) {
		nachfolger = theNachfolger ;
	}
	
	public void setSubContent(BlockContent theSubContent) {
		subContent = theSubContent ;
	}
	
	/**
	  * @param String (text, link, image, ulist, listpoint)
	  */
	public void setTyp(String string) {
		this.typ = string;
	}
	
	/**
	 * @author Dennis
	 * @param HashTable with Attributes, useful for a group of "Knoten" with similar properties
	 */
	public void setAttributes(Hashtable attributeTable)
	{
		this.attributes = attributeTable ;
	}

	public void addAttribute(String thekey, String thevalue)
	{
		attributes.put(thekey, thevalue) ;
	}
	
	public String getTyp() {
		return this.typ ;
	}
	
	public String getContent() {
		return content ;
	}
	
	public BlockContent getNachfolger() {
		return nachfolger ;
	}
	
	public BlockContent getSubContent() {
		return subContent ;
	}
	
	public Hashtable getAttributeHashtable() {
		return attributes ;
	}
	
	public Enumeration getAttributeKeys() {
		return attributes.keys() ;
	}
}
