/*
 * Created on 08.10.2004
 *
 * Created for JAiConect project :Q
 */
package de.ai.jaiconect;

/**
 * 
 * 
 * class JAiNode.java created by @author your mama or at his system
 * created on 08.10.2004 2004 at 16:14:56 
 */
public class JAiNode {

	/**
	 * Constructor for JAiNode object
	 * 
	 * 
	 */
	public JAiNode() {
		super();
		// TODO Auto-generated constructor stub
	}
	
	public void testNode() throws Exception {
		System.out.println("testNode is invoken!!!");
		System.out.println("testNode something done in testNode...");
		System.out.println("testNode is over!!!");
	}
	
	public String nodePrint(String type) {
		System.out.println("ich bin ein "+type);
		return type;
	}

}
