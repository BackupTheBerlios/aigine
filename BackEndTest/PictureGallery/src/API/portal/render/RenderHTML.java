/*
 * Created on 02.01.2005
 *
 */
package API.portal.render;

import java.util.Enumeration;
import java.util.Hashtable;

import API.portal.model.Block;
import API.portal.model.BlockContent;

/**
 * Description: Diese Render-Klasse erzeugt anhand der Blockinhalte (abstrakte Datenstrukturen in Baumform)
 * 				HTML-Code für die Auslieferung an einen Browser
 * @author drichter
 * @since 2005-01-01
 * 
 */
public class RenderHTML implements Render {
	// private Block theBlock = null ;
	Hashtable templates = new Hashtable() ;
	// LATER kein Konstruktor notwendig, für Templateanwendung dann vielleicht doch ... 
	// TODO (byDR) mit beim Laden zu wählenden templates laufen lassen
	
	/**
	 * Description: Konstruktor von RenderHTML, erzeugt ein Standardtemplate mit den nötigen Elementen
	 *
	 */
	public RenderHTML() {
		TemplateHTML stdtemplate = new TemplateHTML() ;
		StringBuffer sb = new StringBuffer() ;
		
		System.out.println("==> API.portal.Render.RenderHTML.constructor") ;
		
		sb.append("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">") ;
		sb.append("<html>\n<head>\n<title>" + "Catalog-Portal für IM (2005-02)" + "</title>\n");
		sb.append("\t<link rel=\"stylesheet\" href=\"catalog.css\" type=\"text/css\" />\n");
		sb.append("</head>\n<body>\n");
		sb.append("<table class=\"site\">\n") ;
		stdtemplate.setHead(sb.toString()) ;
		
		sb = new StringBuffer("") ; // 1
		sb.append("<tr>\n\t<td width=\"100%\" class=\"head\" colspan=\"3\">\n<table border=0>") ;
		stdtemplate.addFrameStart(sb.toString()) ;
		
		sb = new StringBuffer("") ; // 1
		sb.append("</table></td></tr>\n") ;
		stdtemplate.addFrameEnd(sb.toString()) ;
		
		sb = new StringBuffer("") ; // 2
		sb.append("<tr><td width=\"150\">\n<table border=0>") ;
		stdtemplate.addFrameStart(sb.toString()) ;
		
		sb = new StringBuffer("") ; // 3
		sb.append("<td width=\"90%\">\n<table border=0>") ;
		stdtemplate.addFrameStart(sb.toString()) ;
		
		sb = new StringBuffer("") ; // 4
		sb.append("<td width=\"150\">\n<table border=0>") ;
		stdtemplate.addFrameStart(sb.toString()) ;		
		
		sb = new StringBuffer("") ; // 2
		sb.append("</table></td>\n") ;
		stdtemplate.addFrameEnd(sb.toString()) ;

		sb = new StringBuffer("") ;
		sb.append("</tr>\n</table>\n") ;
		sb.append("<div class=\"copyright\" align=center>by DG, FO, DR, TH (C) 2004/2005</div>") ;
		sb.append("</body></html>") ;
		stdtemplate.setFoot(sb.toString()) ;
		
		sb = new StringBuffer("") ;
		sb.append("<tr><td class=\"blocktitle\">theTitle</td></tr><tr><td class=\"blockcontent\"><span class=\"theStyle\">\n") ;
		stdtemplate.addBlockStart(sb.toString()) ;

		sb = new StringBuffer("") ;
		sb.append("\n</span></td></tr>") ;		
		stdtemplate.addBlockEnd(sb.toString()) ;
		
		templates.put(new String("std"), stdtemplate) ;
		System.out.println("<== API.portal.Render.RenderHTML.constructor") ;
	}

	/** 
	 * @author drichter
	 * @since 2005-01-03
	 * @see API.portal.render.Render#getFoot()
	 */
	public Object getFoot(String templatename) {
		String tmp = new String("") ;
		if (templates.containsKey(templatename)) {
			TemplateHTML theTemplate = (TemplateHTML) templates.get(templatename) ; 
			tmp = theTemplate.getFoot() ;
		}
		return tmp;
	}

	/** 
	 * @author drichter
	 * @since 2005-01-03
	 * @see API.portal.render.Render#getHead()
	 */
	public Object getHead(String templatename) {
		String tmp = new String("") ;
		if (templates.containsKey(templatename)) {
			TemplateHTML theTemplate = (TemplateHTML) templates.get(templatename) ; 
			tmp = theTemplate.getHead() ;
		}
		return tmp;
	}

	/**
	 * @see API.portal.render.Render#getBlockStart(java.lang.String)
	 */
	private String getBlockStart(String templatename, int nr) {
		String tmp = new String("") ;
		if (templates.containsKey(templatename)) {
			TemplateHTML theTemplate = (TemplateHTML) templates.get(templatename) ; 
			tmp = theTemplate.getBlockStart(nr) ;
		}
		return tmp;
	}

	/**
	 * @see API.portal.render.Render#getBlockEnd(java.lang.String)
	 */
	private String getBlockEnd(String templatename, int nr) {
		String tmp = new String("") ;
		if (templates.containsKey(templatename)) {
			TemplateHTML theTemplate = (TemplateHTML) templates.get(templatename) ; 
			tmp = theTemplate.getBlockEnd(nr) ;
		}
		return tmp;
	}

	/**
	 * @see API.portal.render.Render#getFrameStart(java.lang.String, int)
	 */
	public Object getFrameStart(String templatename, int nr) {
		String tmp = new String("") ;
	//	System.out.println("  > Suche FrameStart-Element Nr. " + nr + " im Template '" + templatename + "' ...") ;
		if (templates.containsKey(templatename)) {
			TemplateHTML theTemplate = (TemplateHTML) templates.get(templatename) ; 
			tmp = theTemplate.getFrameStart(nr) ;
	//		System.out.println("+++ gefunden !!! Element hat den Wert: '" + tmp + "'") ;
		}
		return tmp;
	}

	/**
	 * @see API.portal.render.Render#getFrameEnd(java.lang.String, int)
	 */
	public Object getFrameEnd(String templatename, int nr) {
		String tmp = new String("") ;
		if (templates.containsKey(templatename)) {
			TemplateHTML theTemplate = (TemplateHTML) templates.get(templatename) ; 
			tmp = theTemplate.getFrameEnd(nr) ;
		}
		return tmp;
	}
	

	
	/**
	 * @return Das Ergebnis des Rendern wird zurückgeliefert, bei HTML ist das ein String
	 * gibt den fertigen HTML-Inhalt dieses Frames zurück :-)
	 */

	public Object getRenderResult(Hashtable blocks, String templateName) {
		StringBuffer HTMLstring = new StringBuffer("") ;
	//	theBlock = rootBlock ;
		int zaehler = 0 ;
		
		System.out.println("==> API.portal.Render.RenderHTML.getRenderResult\n\n") ;
//		System.out.println(blocks + "\n\n") ;
		// HTMLstring.append("<table border=0>") ;
		while (blocks.containsKey(new Integer(zaehler))) {
			// TODO !!! hier muss der Blockkopf eingefügt werden
			System.out.println("  > verarbeite einen Block (Nummer: " + zaehler + ")") ;
			Block b = (Block) blocks.get(new Integer(zaehler)) ;
			
			StringBuffer sb = new StringBuffer(this.getBlockStart(templateName, 1)) ; // TODO (byDR) unflexible BlockStart/BlockEnd-Verwendung ändern!!
			System.out.println("  > BlockStart: '" + sb + "'") ;
			if (sb.indexOf("theTitle") > -1) {
				sb.replace(sb.indexOf("theTitle"), sb.indexOf("theTitle") + 8, b.getTitle()) ;
			}
			if (sb.indexOf("theStyle") > -1) {
				sb.replace(sb.indexOf("theStyle"), sb.indexOf("theStyle") + 8, b.getStyle()) ;
			}
			HTMLstring.append(sb) ;
//			sb.append("<span class=\"" + theBlock.getStyle() + "\">") ;
			HTMLstring.append(parseContent(b.getContent())) ;
			
			HTMLstring.append(this.getBlockEnd(templateName, 1)) ;
			zaehler++ ;
		}
//		HTMLstring.append("<tr><td class=\"blocktitle\">" + b.getTitle() + "</td></tr><tr><td class=\"blockcontent\">" + b.getcontentHTML() + "</td></tr>");
//		HTMLstring.append("</table>") ;
//		HTMLstring.append(parseContent(rootBlock.getContent())) ;
		System.out.println("<== API.portal.Render.RenderHTML.getRenderResult") ;
		return HTMLstring.toString() ; 
	}

	private String parseContent(BlockContent bc) {
		StringBuffer sb = new StringBuffer() ;
		// TODO (byDR) Ausgaben wieder rausnehmen
		System.out.println("==> API.control.web.Block.parseContentToHTML") ;
		System.out.println("  > beginne mit dem Bauen ...") ;
//			sb.append("<div class=\"" + this.style + "\">" + bc.getContent() + "</div") ;
// TODO hier muss das Verwenden der Attribut-Tabelle integriert werden, ausserdem weitere Anzeigetypen wie Links und Bilder,
//		das Verwenden der Attribute sollte gleich generisch für die verschiedene Knotentypen umgesetzt werden
//		dann kann die erste Probeversion der Galerie ausgegeben werden 

		while (bc != null) {
			if (bc.getTyp().compareTo(new String("text")) == 0) {
				sb.append("<div" + extractAttributes(bc) + ">" + bc.getContent()) ;
				if (bc.getSubContent() != null) {
					System.out.println("  > SubContent ...") ;
					sb.append(parseContent(bc.getSubContent())) ;
					// rekursives Durchlaufen
				}
				sb.append("</div>\n") ;
			} else if (bc.getTyp().compareTo(new String("image")) == 0) {
				sb.append("<img" + extractAttributes(bc) + " />\n") ;
				if (bc.getSubContent() != null) {
					System.out.println("  > SubContent ...") ;
					sb.append(parseContent(bc.getSubContent())) ;
					// rekursives Durchlaufen
				}
			} else if (bc.getTyp().compareTo(new String("link")) == 0) {
				sb.append("<a" + extractAttributes(bc) + ">" + bc.getContent()) ;
				if (bc.getSubContent() != null) {
					System.out.println("  > SubContent ...") ;
					sb.append(parseContent(bc.getSubContent())) ;
					// rekursives Durchlaufen
				}
				sb.append("</a>\n") ;
			} else if (bc.getTyp().compareTo(new String("ulist")) == 0) {
				sb.append("<ul" + extractAttributes(bc) + ">" + bc.getContent()) ;
				if (bc.getSubContent() != null) {
					System.out.println("  > SubContent ...") ;
					sb.append(parseContent(bc.getSubContent())) ;
					// rekursives Durchlaufen
				}
				sb.append("</ul>\n") ;
			} else if (bc.getTyp().compareTo(new String("listpoint")) == 0) {
				sb.append("<li" + extractAttributes(bc) + ">" + bc.getContent()) ;
				if (bc.getSubContent() != null) {
					System.out.println("  > SubContent ...") ;
					sb.append(parseContent(bc.getSubContent())) ;
					// rekursives Durchlaufen
				}
				sb.append("</li>\n") ;
			}
			
//			TODO (byDR) hier muessen weitere Typen wie ?navigation?, table umgesetzt werden


			bc = bc.getNachfolger() ;
			System.out.println("  > nächster Content-Knoten ...") ;
		}
		System.out.println("<== API.control.web.Block.parseContentToHTML") ;
		return sb.toString() ;
	}

	/**
	 * Description: Diese private Methode erstellt aus den Attributen der Hasttable eines BlockContent-Objektes eine XHTML-Attributliste als
	 * String
	 * @param BlockContent
	 * @author drichter
	 * @since 2005-01-03
	 * */
	private String extractAttributes(BlockContent bc) {
		StringBuffer sb = new StringBuffer("") ;
		String theKey = null ;
		String theValue = null ;
		Hashtable theAttribs = bc.getAttributeHashtable() ;
		if (theAttribs != null) {
			for (Enumeration enum2 = bc.getAttributeKeys() ; enum2.hasMoreElements(); ) {
				theKey = (String) enum2.nextElement() ;
				theValue = (String) theAttribs.get(theKey) ;
				sb.append(" " + theKey + "=\"" + theValue + "\"") ;
			}
		}
		return sb.toString() ;
	}

}
