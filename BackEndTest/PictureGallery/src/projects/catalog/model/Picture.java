/*
 * Created on 07.09.2004
 *
 * 
 */
package projects.catalog.model;

import java.io.Serializable;

/**
 * @author your mama
 *
 * Persistence class to handle picture- persistence
 */
public class Picture implements Serializable {
	
	private long id;
	private String name;
	private String format;
	private Category thema;
	private String src_path;
	
	public Picture(String thepath, String theformat, String thename) {
		this.src_path = thepath ;
		this.format = theformat ;
		this.name = thename ;
	}

	/**
	 * @return Returns the format.
	 */
	public String getFormat() {
		return format;
	}
	/**
	 * @param format The format to set.
	 */
	public void setFormat(String format) {
		this.format = format;
	}
	/**
	 * @return Returns the id.
	 */
	public long getId() {
		return id;
	}
	/**
	 * 
	 * @param id The id to set.
	 */
	public void setId(long id) {
		this.id = id;
	}
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
	
	public String getPath() {
		return this.src_path ;
	}
	
	/**
	 * ToHTML methode die das bild erstmal in einer einfachen Tabellen structur ausgibt
	 * 
	 * 
	 * class Picture.java created by @author your mama or at his system
	 * created on 10.09.2004 2004 at 06:14:39
	 */
/*	public String toHtml() {
		StringBuffer pic_html = new StringBuffer();
		pic_html.append("<td> <img src="+src_path+"");
		return pic_html.toString();
	}*/
}
