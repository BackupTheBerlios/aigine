/*
 * Created on 16.09.2004
 *
 * To change the template for this generated file go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
package API.control.web;

import java.util.Hashtable;

/**
 * @author Dennis
 * @since 2004-09-16
 * enthält ein 'Frameset', die Reihenfolge wird mit Positionierung im Basis-Template
 * verbunden.
 */
public class Frames {
	private Hashtable frames = new Hashtable();
	
	Frames() {
		// TODO hier muss die FrameStruktur des Benutzers initialisiert werden,
		// vielleicht am besten aussen lesen lassen und dann hier Frame für Frame
		// eintragen
		
	}

	/**
		 * Fügt einen Frame hinzu.
		 * @param newFrame, FrameNumber
	 */
	public int addFrame(BlockFrame newFrame, int FrameNumber) {
		frames.put(new Integer(FrameNumber), newFrame);
		return 0 ;
	}

	/**
	 * Fügt einen Frame hinzu.
	 * @param newFrame, FrameNumber
	 */
	public BlockFrame getFrame(int FrameNumber) {
		BlockFrame fr = (BlockFrame) frames.get(new Integer(FrameNumber)) ;
		return fr ;
	}
}
