/*
 * Created on 16.09.2004
 *
 */
package API.portal.model;

import java.util.Hashtable;

/**
 * @author Dennis
 * @since 2004-09-16
 * enthält ein 'Frameset', die Reihenfolge wird mit Positionierung im Basis-Template
 * verbunden.
 */
public class Frames {
	private Hashtable frames = new Hashtable();

	public Frames() {
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
