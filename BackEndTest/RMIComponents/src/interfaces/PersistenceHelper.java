/*
 * Created on 16.08.2004
 *
 * To change the template for this generated file go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
package interfaces;


/**
 * @author tobi
 *
 * To change the template for this generated type comment go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
public interface PersistenceHelper {

	public void save() ;
	
	
	 /**
	* Holt die Vote Obj. aus dem Hashtable, liest den count und
	* speichert ihn in der Configuration (jconfig)
	*
	*/   
   public void refreshConfiguration();
   
//   VoteTable getVoteTable() ;
//   
//   public List listVotes();

}
