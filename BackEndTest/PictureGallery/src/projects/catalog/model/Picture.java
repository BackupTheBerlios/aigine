/*
 * Created on 07.09.2004
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package projects.catalog.model;

/**
 * @author your mama
 *
 * Persistenc class to handle picture- persistence
 */
public class Picture {
	
	private long id;
	private String name;
	private String format;
	private Category thema;
	private String src_path;
	

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
	 * private to let hiberate handle the id
	 * @param id The id to set.
	 */
	private void setId(long id) {
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
	
	/**
	 * ToHTML methode die das bild erstmal in einer einfachen Tabellen structur ausgibt
	 * 
	 * 
	 * class Picture.java created by @author your mama or at his system
	 * created on 10.09.2004 2004 at 06:14:39
	 */
	public String toHtml() {
		StringBuffer pic_html = new StringBuffer();
		pic_html.append("<td> <img src="+src_path+"");
		return pic_html.toString();
	}
}
