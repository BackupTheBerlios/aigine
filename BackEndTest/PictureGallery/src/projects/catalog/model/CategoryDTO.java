/*
 * Created on 2005-03-19
 *
 */
package projects.catalog.model;

import java.util.Date;

import API.model.AbstractDTO;

/**
 * @author drichter
 *
 * 
 */
public class CategoryDTO extends AbstractDTO {
	private String name ;
	private String description ;
	private Date created ;


	/**
	 * @return Returns the name.
	 */
	public String getName() {
		return name;
	}
	/**
	 * @param name The name to set.
	 */
	public void setName(String name) {
		this.name = name;
	}
	/**
	 * @return
	 */
	public Date getCreated() {
		return created;
	}

	/**
	 * @return
	 */
	public String getDescription() {
		return description;
	}

	/**
	 * @param date
	 */
	public void setCreated(Date date) {
		created = date;
	}

	/**
	 * @param string
	 */
	public void setDescription(String string) {
		description = string;
	}

}
