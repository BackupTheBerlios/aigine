/*
 * Created on 16.09.2004
 *
 * To change the template for this generated file go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
package API.control.web;

import java.io.Serializable;

/**
 * @author Dennis
 * @since 2004-09-16
 * 
 */
public class BlockContent implements Serializable {
// Diese Klasse dient dem Aufbau hierarchischer Block-Strukturen, jeder Knoten
// kann Unterknoten enthalten
	private BlockContent nachfolger = null ;
	private BlockContent subContent = null ;
	// subContent enthält eine untergeordnete Kette, dies erlaubt Tabellen, Links u.ä.
	private String content = new String("leerer Content-Knoten") ;
		
	public BlockContent (String thecontent) {
		this.content = thecontent ;
	}
	
	public void setNachfolger(BlockContent theNachfolger) {
		nachfolger = theNachfolger ;
	}
	
	public void setSubContent(BlockContent theSubContent) {
		subContent = theSubContent ;
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
}
