/*
 * Created on 07.09.2004
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package projects.catalog.model;

import java.util.Vector;

/**
 * @author your mama
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public class Warenkorb {
	private long id;
	private long uid;
	private Vector things;
	
	/**
	 * @return Returns the id.
	 */
	public long getId() {
		return id;
	}
	/**
	 * @param id The id to set.
	 */
	public void setId(long id) {
		this.id = id;
	}
	/**
	 * @return Returns the things.
	 */
	public Vector getThings() {
		return things;
	}
	/**
	 * @param things The things to set.
	 */
	public void setThings(Vector things) {
		this.things = things;
	}
}
