/*
 * Created on 16.08.2004
 * File: package API.control.web;.Block.java
 */
package API.control.web;

import java.io.Serializable;

/**
 * Ein Block kann in einem Frame des Portals dargestellt werden.
 * @author danny, Dennis
 * @since 16.08.2004 06:20:57
 * @version 0.02
 */
public class Block implements Serializable {
	private String title = "BLOCK TITLE";
	private BlockContent content = null ;
	private String template = null ;
	private String typ = new String("text") ;
	private String style = new String("catalog") ;

	public Block(BlockContent bc) {
		content = bc ;
	}
	
	public Block(String simpleContent) {
		content = new BlockContent(simpleContent) ;
		// typ = text und style = catalog
	}
	
	public void setBlockTemplate(String thetemplate) {
		this.template = thetemplate ;
	}
	
	// hierzu gibt es keine Getter-Funktion
	public void setContent(BlockContent bc) {
		content = bc ;
	}

    /**
     * @return
     */
    public String getcontentHTML() {
    	StringBuffer HTMLstring = new StringBuffer("") ;
        HTMLstring.append(parseContentToHTML(content)) ;  		
        return HTMLstring.toString() ; 
    }

	private String parseContentToHTML(BlockContent bc) {
		StringBuffer sb = new StringBuffer() ;
		// TODO (byDR) Ausgaben wieder rausnehmen
		System.out.println("==> API.control.web.Block.parseContentToHTML") ;
		if (this.typ.compareTo(new String("text")) == 0) {
			System.out.println("  > beginne mit dem Bauen ...") ;
//			sb.append("<div class=\"" + this.style + "\">" + bc.getContent() + "</div") ;
			while (bc != null) {
				sb.append("<div class=\"" + this.style + "\">" + bc.getContent() + "</div>") ;
				if (bc.getSubContent() != null) {
					System.out.println("  > SubContent ...") ;
					sb.append(parseContentToHTML(bc.getSubContent())) ;
					// rekursives Durchlaufen
				}
				bc = bc.getNachfolger() ;
				System.out.println("  > nächster Content-Knoten ...") ;
			}  ;
		} else {
			// TODO (byDR) hier muessen weitere Typen wie navigation, list, table umgesetzt werden			
		}
		System.out.println("<== API.control.web.Block.parseContentToHTML") ;
		return sb.toString() ;
	}


	/**
	 * @return
	 */
	public String getcontentXML() {
		return new String("XML-Code eines Blocks") ;
	}

    /**
     * @return the Title of the Block
     */
    public String getTitle() {
        return title;
    }

    /**
     * @param string
     */
    public void setTitle(String string) {
        title = string;
    }

	/**
	  * @param string
	  */
	 public void setTyp(String string) {
		 this.typ = string;
	 }
	 
	/**
	  * @param string (wird als CSS-Klasse eingesetzt)
	  */
	 public void setStyle(String string) {
		 this.style = string;
	 }
}
