/*
 * Created on 16.09.2004
 *
 * To change the template for this generated file go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
package API.portal.model;

import java.util.Hashtable;

/**
 * @author Dennis
 *
 * 
 */
public class RequestFrame {
	private Hashtable requestblocks ;
	private int number = 0 ;

	public RequestFrame(int frameNumber) {
		requestblocks = new Hashtable() ;
		this.number = frameNumber ;
	}
	
	/**
	 * @return Anzahl der Frames
	 */
	public int getBlockCount() {
		int zaehler = 0 ;
//		System.out.print("\n  > zaehle Bloecke: ") ;
		while (requestblocks.get(new Integer(zaehler)) != null) {
			zaehler++ ;
//			System.out.print(", " + zaehler) ;
		}
//		System.out.print("\n\n") ;
		return zaehler ;
	}
	
	public int getFrameNumber() {
		return number ;
	}
	
	public void addBlock(String BlockName, String server, String operation, int blocknumber) {
		requestblocks.put(new Integer(blocknumber), new RequestBlock(BlockName, server, operation)) ;
	}
	
	public void addBlock(String Blockname, String server, String operation, Hashtable requestProps, int blocknumber) {
		// TODO (byDR) implementieren!
				
	}
	
	public RequestBlock getRequestBlock(int BlockNumber){
		RequestBlock rb = (RequestBlock) requestblocks.get(new Integer(BlockNumber)) ;
		return rb ; 
	}

}