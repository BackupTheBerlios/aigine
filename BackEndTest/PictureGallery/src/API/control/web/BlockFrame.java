/*
 * Created on 16.08.2004
 * File: package API.control.web;.BlockFrame.java
 */
package API.control.web;

import java.util.Enumeration;
import java.util.Hashtable;

/**
 * Nimmt Blocks auf.
 * @author danny
 * @since 16.08.2004 06:18:52
 * @version 0.01
 */
public class BlockFrame {
	private Hashtable blocks = new Hashtable();
	/**
	 * Fügt einen Block hinzu.
	 * @param block
	 */
	public void addBlock(Block block){
		// TODO das muss geordnet stattfinden können?!
		//		besser wäre es wohl schon
		//blocks.put(block.getTitle(), block);
		blocks.put(new Integer(blocks.size()), block);
	}
	
	/**
	 * @author Dennis 
	 * @since 2004-09-16
	 * @return gibt den fertigen HTML-Inhalt dieses Frames zurück :-)
	 * @param keine
	 */
	public String getBlocks() {
		StringBuffer sb = new StringBuffer();
		int zaehler = 0 ;
		sb.append("<table border=0>") ;
		while (blocks.get(new Integer(zaehler)) != null) {
			Block b = (Block) blocks.get(new Integer(zaehler)) ;
			sb.append("<tr><td class=\"blocktitle\">" + b.getTitle() + "</td></tr><tr><td class=\"blockcontent\">" + b.getcontentHTML() + "</td></tr>");
			zaehler++ ;
		}
		sb.append("</table>") ;
		return sb.toString() ;
	}

	/**
	 * Gibt alle Blocks in einer Table aus.
	 * @return
	 */
	public String getContent(){
		StringBuffer bs = new StringBuffer();
		bs.append("<table><tr>");
		Enumeration enum = blocks.elements();
		while(enum.hasMoreElements()){
			Block b = (Block)enum.nextElement();
//			bs.append("<td>" + b.getTitle() + "<hr>" + b.getContent() + "</td>");
		}
		bs.append("</tr></table>");
		return bs.toString();
	}
}
