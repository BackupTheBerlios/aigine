/*
 * Created on 07.09.2004
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package projects.interfaces;

import org.jconfig.Category;

import projects.catalog.model.Catalog;
import projects.catalog.model.Picture;
import projects.catalog.model.User;

import API.control.Session;
import API.interfaces.PersistenceHelper;

/**
 * @author your mama
 *
 * TODO soll eine reie von datenbank-anfragen handeln wie z.B:
 * 	- getUser(userID) einen einzelnen user aus der db nach seiner id fischen
 *  - getUser(CSession) fieleicht noch nach der session
 *  - setUser(User usr, CSession session) einen user in die db schreiben
 * 	- setPicture(Picture pic) ;
 *  
 */
public interface CPersistenceHelper extends PersistenceHelper {
	
	Session session = null;
	
	User getUser(long userID); // einen einzelnen user aus der db nach seiner id fischen
	
	User getUser(Session session); // fieleicht noch nach der session
	
	void setUser(User usr, Session session); // einen user in die db schreiben
	
	
	/**
	 * setzt einen Catalog. dieser beinhaltet logisch mehere Categorien
	 * welche dann wiederum die bilder enthalten.
	 * non-overriding method for CPersistenceHelper
	 * Tags are:@param catalog
	 * @return void
	 */
	void setCatalog(Catalog catalog); 
	
	/**
	 * setzt eine neue Category in die db und in einen bestehenden Catalog
	 * non-overriding method for CPersistenceHelper
	 * Tags are:@param category
	 * @return void
	 */
	void setCategory(Category category);
	
	void setPicture(Picture pic, Session session);
	
}
