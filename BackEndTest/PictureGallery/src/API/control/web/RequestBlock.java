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
 * To change the template for this generated type comment go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
public class RequestBlock {
	private String op = null ;
	private String srv = null ;
	private String name = null ;
	Hashtable requests = null ;
	
	public RequestBlock(String BlockName, String server, String operation) {
		this.srv = server ;
		this.op = operation ;
		this.name = BlockName ;
	}
	
	public RequestBlock(String BlockName, String server, String operation, Hashtable requestProps) {
		this.requests = requestProps ;
		this.srv = server ;
		this.op = operation ;
		this.name = BlockName ;
	}
	
	public String getServer() {
		return this.srv ;
	}
	
	public String getOperation() {
		return this.op ;
	}
	
	public Hashtable getRequests() {
		return this.requests ;
	}
	
	public String getName() {
		return this.name ;
	}
}
