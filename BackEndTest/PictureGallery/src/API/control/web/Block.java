/*
 * Created on 16.08.2004
 * File: package API.control.web;.Block.java
 */
package API.control.web;

/**
 * Ein Block kann im linken und rechten Frame des Portals 
 * dargestellt werden.
 * @author danny
 * @since 16.08.2004 06:20:57
 * @version 0.01
 */
public class Block {
	private String title = "BLOCK TITLE";
	private String content ="BLOCK CONTENT";

    /**
     * @return
     */
    public String getContent() {
        return content;
    }

    /**
     * @return
     */
    public String getTitle() {
        return title;
    }

    /**
     * @param string
     */
    public void setContent(String string) {
        content= string;
    }

    /**
     * @param string
     */
    public void setTitle(String string) {
        title= string;
    }

}
