/*
 * Created on 16.09.2004
 *
 */
package API.control.web;

import java.util.Hashtable;
/**
 * @author Dennis
 * @since 2004-09-16
 * enthält ein 'Frameset', die Reihenfolge wird mit Positionierung im Basis-Template
 * verbunden. *
 */
public class RequestFrames {

	private Hashtable frames = new Hashtable();

	public RequestFrames() {
	}
	/**
		 * Fügt einen Frame hinzu.
		 * @param newFrame, FrameNumber
	 */
	public int addFrame(RequestFrame newFrame, int FrameNumber) {
		frames.put(new Integer(FrameNumber), newFrame);
		return 0 ;
	}

	/**
	 * Fügt einen Frame hinzu.
	 * @param newFrame, FrameNumber
	 */
	public RequestFrame getFrame(int FrameNumber) {
		RequestFrame fr = (RequestFrame) frames.get(new Integer(FrameNumber)) ;
		return fr ;
	}
}
