/*
 * Created on 15.08.2004
 *
 * To change the template for this generated file go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
package projects.voting;

/**
 * @author tobi
 *
 * To change the template for this generated type comment go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
public class VTDatabaseImpl {

	/**
	 * 
	 */
	public VTDatabaseImpl() {
		System.out.println("=>HSQLServerImpl.constructor\n");
		System.out.println("HSQLServer starten mit der in der properties spezialiseierten Datenbank");
		Server hsqlserver = new Server();
		System.out.println("\n<= HSQLServerImpl.constructor\n");
	}

}
