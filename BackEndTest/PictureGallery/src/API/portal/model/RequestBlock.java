/*
 * Created on 16.09.2004
 *
 * To change the template for this generated file go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
package API.portal.model;

import java.util.Hashtable;

import API.model.AbstractDTO;

/**
 * @author Dennis
 * @since 2004-09-16
 *
 */
public class RequestBlock extends AbstractDTO {
	private String op = null ;
	private String srv = null ;
	private String name = null ;
	Hashtable requests = new Hashtable() ;
	
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
	
	public void setServer(String newServer) {
		this.srv = newServer ;
	}

	public void setOperation(String newOperation) {
		this.op = newOperation ;
	}

	public void setRequests(Hashtable ht) {
		this.requests = ht ;
	}
	
	public String getOperation() {
		return this.op ;
	}

	public String getServer() {
		return this.srv ;
	}	
	
	public Hashtable getRequests() {
		return this.requests ;
	}
	
	public String getName() {
		return this.name ;
	}
}
