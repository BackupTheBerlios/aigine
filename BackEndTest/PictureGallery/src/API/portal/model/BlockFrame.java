/*
 * Created on 16.08.2004
 * File: package API.control.web;.BlockFrame.java
 */
package API.portal.model;

import java.util.Hashtable;

/**
 * Nimmt Blocks auf.
 * @author danny, drichter
 * @since 16.08.2004 06:18:52
 * @version 0.1
 */
public class BlockFrame {
	private Hashtable blocks = new Hashtable();

	/**
	 * Fügt einen Block hinzu. Die eingehende Reihenfolge bestimmt die nummerierte Keyvergabe.
	 * @param block
	 */
	public void addBlock(Block block) {
		blocks.put(new Integer(blocks.size()), block); // immer an letzter Stelle einfügen
	}
	
	/**
	 * @author Dennis 
	 * @since 2004-09-16
	 * @return die Blöcke des BlockFrames als Hashtable mit sortierten, hochzählenden Keys
	 * @param keine
	 */
	public Hashtable getBlocks() {
		return blocks ;
	}

	/**
	 * Gibt alle Blocks in einer Table aus.
	 * @return
	 */
/*	public String getContent(){
		StringBuffer bs = new StringBuffer();
		bs.append("<table><tr>");
		Enumeration enum1 = blocks.elements();
		while(enum1.hasMoreElements()){
			Block b = (Block)enum1.nextElement();
//			bs.append("<td>" + b.getTitle() + "<hr>" + b.getContent() + "</td>");
		}
		bs.append("</tr></table>");
		return bs.toString();
	} */
	
}
