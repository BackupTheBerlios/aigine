/*
 * Created on 16.08.2004
 * File: package API.control.web;.BlockFrame.java
 */
package API.control.web;

import java.util.Hashtable;

/**
 * Nimmt Blocks auf.
 * @author danny
 * @since 16.08.2004 06:18:52
 * @version 0.01
 */
public class BlockFrame {
	private Hashtable blocks = new Hashtable();
	public void addBlock(Block block){
		blocks.put(block.getTitle(), block);
	}
}
