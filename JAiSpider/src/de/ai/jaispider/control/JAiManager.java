/*
 * Created on 06.10.2004
 *
 */
package de.ai.jaispider.control;

import java.rmi.server.RemoteObject;


/**
 * Bietet für den JAiLoader die Node- Bibliothek und für diese
 * wiederum das Klassenverzeichnis, die Bibliotheken und die 
 * zugehörigen Konfigurationsdateien an.
 * 
 * @author danny
 *
 */
public class JAiManager {

	
	//private nodeTree -> to store the single nodes as a tree ( TODO anable node´s, to register at node)
	//					-> TODO create a DefaultTreeModel for the view
	//private nodeTable -> store node´s as a Table (TODO ceate DefaultTebleModel for the view)
	//private nodeList -> für einen logischen graphen(doppelt verkette liste). würde ebenfalls vorraussetzten,
	//					dass sich einzelne Knoten untereinander regestrieren können.
	
	/**
	 * Manager sollte folgende Funktionalität mitbringen:
	 * 	- register(JAiNode node)
	 *  - registerAdmin(RemoteObject adminClient)
	 * 	- getNode(String name) 		-> search for a node by his name 
	 *  - getNode(int x, int y); 	-> serch for a node by his coordinates in the saved graph
	 *  - getResorcces("<package deklaration like: "java.net.*"> ") -> serch for the given classes in
	 * 		his .jar libaries
	 *  - getClass("classpath") -> sucht nach einer einzelnen Klasse in den jar bibliotheken
	 *  - 
	 */
	public JAiManager() {
		super();
		// TODO Auto-generated constructor stub
	}

	public static void main(String[] args) {
		
	}
	
	/**
	 * Register a given Node at the Manager- Note-Tree (table)
	 * non-overriding method for JAiManager
	 * Tags are:@param rmoteObj
	 * @return void
	 */
	public synchronized void register(JAiNode node) {
		
	}
	
	/**
	 * 
	 * non-overriding method for JAiManager
	 * Tags are:@param adminNode
	 * @return void
	 */
	public synchronized void registerAdmin(JAiNode adminNode) {
		
	}
	
	
}
