/*
 * Created on 07.09.2004
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package projects.catalog.model;

import API.control.Session;

/**
 * @author your mama
 * 
 * Helperclass for user- persistens and session- handle
 */
public class User {
	private long id;
	private String firstname;
	private String lastname;
	private String nicname;
	private Session session;
	
	
	

	/**
	 * @return Returns the firstname.
	 */
	public String getFirstname() {
		return firstname;
	}
	/**
	 * @param firstname The firstname to set.
	 */
	public void setFirstname(String firstname) {
		this.firstname = firstname;
	}
	/**
	 * @return Returns the lastname.
	 */
	public String getLastname() {
		return lastname;
	}
	/**
	 * @param lastname The lastname to set.
	 */
	public void setLastname(String lastname) {
		this.lastname = lastname;
	}
	/**
	 * @return Returns the sessionID.
	 */
	public Session getSession() {
		return session;
	}
	/**
	 * @param sessionID The sessionID to set.
	 */
	public void setSession(Session session) {
		this.session = session;
	}
	/**
	 * private to let only hibernate handle the id
	 * @param id The id to set.
	 */
	private void setId(long id) {
		this.id = id;
	}
	/**
	 * @return Returns the id.
	 */
	public long getId() {
		return id;
	}
}
