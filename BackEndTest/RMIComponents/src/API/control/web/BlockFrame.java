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
		blocks.put(block.getTitle(), block);
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
			bs.append("<td>" + b.getTitle() + "<hr>" + b.getContent() + "</td>");
		}
		bs.append("</tr></table>");
		return bs.toString();
	}
}
