/*
 * Created on 08.10.2004
 *
 * Created for JAiConect project :Q
 */
package de.ai.jaispider.control;

/**
 * 
 * 
 * class JAiNode.java created by @author your mama or at his system
 * created on 08.10.2004 2004 at 16:14:56
 * 
 * Der jaiNode ist ein element innerhalb der jevaligen anwendung
 * je nach art und weise der aufgaben sollte ein Node sowohl als 
 * Client, aber auch (wie der name schon sagt) als Knoten in einem
 * Netzwerk oder Baum. Hierfür ist es von nöten, den node auch als 
 * Sub- Server zu verstehen, der wiederrum regestrierte Knoten verwalten kann
 * 
 *  
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
	
	public void registerNode(JAiNode node) {
		//TODO eine register- methode, die sowohl eine eigene knoten- liste 
		//		pflegt, als auch dem manager von seiner existens und beschaffenheit
		// 		in kenntnis setzt
		
	}
	
	
	
	
	
	
	
	//Testing the test- system JUnit...
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
