/*
 * Created on 07.09.2004
 * Rewritten on 20050318
 * 
 */
package projects.catalog.model;

import java.io.Serializable;
import java.util.Date;

import API.model.AbstractDTO;

/**
 * @author drichter
 *
 * Persistence class to handle picture- persistence
 */
public class PictureDTO extends AbstractDTO implements Serializable {
	
	private String user_id ;
	private String titel ;
	private String comment ;
	private Date created ;
	private Date upload ; 
	
	public PictureDTO() {
		
	}
	
	public PictureDTO(String theID, String userID, String theTitel, String theComment, Date theCreateDate, Date theUploadDate) {
		this.setID(theID) ;
		this.user_id = userID ;
		this.titel = theTitel ;
		this.comment = theComment ;
		this.created = theCreateDate ;
		this.upload = theUploadDate ;
	}

	/**
	 * @return
	 */
	public String getComment() {
		return comment;
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
	public String getTitel() {
		return titel;
	}

	/**
	 * @return
	 */
	public Date getUpload() {
		return upload;
	}

	/**
	 * @param string
	 */
	public void setComment(String string) {
		comment = string;
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
	public void setTitel(String string) {
		titel = string;
	}

	/**
	 * @param date
	 */
	public void setUpload(Date date) {
		upload = date;
	}

	/**
	 * @return
	 */
	public String getUser_id() {
		return user_id;
	}

	/**
	 * @param string
	 */
	public void setUser_id(String string) {
		user_id = string;
	}

}
