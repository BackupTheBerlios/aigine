/*
 * Created on 02.01.2005
 * @author drichter
 * 
 */
package API.portal.render;

import java.util.Hashtable;

/**
 * @author drichter
 * Description: Interface for different Render-Classes used by PortalServer
 * 
 */
public interface Render {
	
	public Object getRenderResult(Hashtable blocks, String templateName) ;
	public Object getHead(String templatename) ;
	public Object getFoot(String templatename) ;
//	public Object getBlockStart(String templatename) ;
//	public Object getBlockEnd(String templatename) ;
	public Object getFrameStart(String templatename, int nr) ;
	public Object getFrameEnd(String templatename, int nr) ;
}
