/*
 * Created on 16.09.2004
 *
 * To change the template for this generated file go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
package API.control.web;

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
	
	public int getFrameNumber() {
		return number ;
	}
	
	public void addBlock(String BlockName, String server, String operation, int blocknumber) {
		requestblocks.put(new Integer(blocknumber), new RequestBlock(BlockName, server, operation)) ;
	}
	
	public void addBlock(String server, String operation, Hashtable requestProps, int blocknumber) {
		// TODO (byDR) implementieren!
				
	}
	
	public RequestBlock getRequestBlock(int BlockNumber){
		RequestBlock rb = (RequestBlock) requestblocks.get(new Integer(BlockNumber)) ;
		return rb ; 
	}

}