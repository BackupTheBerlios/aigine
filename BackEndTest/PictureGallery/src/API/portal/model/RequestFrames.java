/*
 * Created on 16.09.2004
 *
 */
package API.portal.model;

import java.util.Hashtable;
/**
 * @author Dennis
 * @since 2004-09-16
 * enth�lt ein 'Frameset', die Reihenfolge wird mit Positionierung im Basis-Template
 * verbunden. *
 */
public class RequestFrames {

	private Hashtable frames = new Hashtable();

	public RequestFrames() {
	}
	
	/**
	 * @return Anzahl der Frames
	 */
	public int getFrameCount() {
		int zaehler = 0 ;
		while (frames.get(new Integer(zaehler)) != null) {
			zaehler++ ;
		}
		return zaehler ;
	}
	
	/**
	 * F�gt einen Frame hinzu.
	 * @param newFrame, FrameNumber
	 */
	public int addFrame(RequestFrame newFrame, int FrameNumber) {
		frames.put(new Integer(FrameNumber), newFrame);
		return 0 ;
	}

	/**
	 * F�gt einen Frame hinzu.
	 * @param newFrame, FrameNumber
	 */
	public RequestFrame getFrame(int FrameNumber) {
		RequestFrame fr = (RequestFrame) frames.get(new Integer(FrameNumber)) ;
		return fr ;
	}
}
