/*
 * Created on 16.08.2004
 * File: package projects.voting.control.web;.Head.java
 */
package API.control.web;

/**
 * Gibt den Header einer Seite aus.
 * Titel der HTML Seite und content können verändert werden.
 * @author danny
 * @since 16.08.2004 05:34:23
 * @version 0.01
 */
public class HeadFrame {
	private String title = "TITLE WEB Testumgebung VSYSBeleg";
	private String content ="CONTENT Willkommen auf der Online Plattform";
	public String getHeader(){
		StringBuffer header = new StringBuffer();
		header.append("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">");
		header.append("<html>\n<head>\n<title>" + title + "</title>\n");
		header.append("\t<link rel=\"stylesheet\" href=\"design.css\" type=\"text/css\" />\n");
		header.append("</head>\n<body>\n<table class=\"site\">\n<tr>\n\t<td colspan=\"3\">\n");
		header.append("<!-- Header -->\n");
		
		header.append(content);
		
		header.append("\t</td>\n</tr>\n<tr>\n");
        return header.toString();
	}
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
