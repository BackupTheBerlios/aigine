/*
 * Created on 16.08.2004
 * File: package API.control.web;.Block.java
 */
package API.portal.model;

import java.io.Serializable;

/**
 * Ein Block kann in einem Frame des Portals dargestellt werden.
 * @author danny, Dennis
 * @since 16.08.2004 06:20:57
 * @version 0.1
 */
public class Block implements Serializable {
	private String title = "BLOCK TITLE";
	private BlockContent content = null ;
	private String style = new String("catalog") ;

	public Block(BlockContent bc) {
		content = bc ;
	}
	
	public Block(String simpleContent) {
		content = new BlockContent(simpleContent) ;
	}
	
	public void setContent(BlockContent bc) {
		content = bc ;
	}

	/**
	 * @author Dennis
	 * @return the content of the Block, used by Render-Classes 
	 */
	public BlockContent getContent() {
		return content ;
	}

    /**
     * @return the Title of the Block
     */
    public String getTitle() {
        return title;
    }

	public String getStyle() {
		return this.style ;
	}

    /**
     * @param string
     */
    public void setTitle(String string) {
        title = string;
    }

	/**
	  * @param string (wird bspw. als CSS-Klasse eingesetzt)
	  */
	 public void setStyle(String string) {
		 this.style = string;
	 }
}
