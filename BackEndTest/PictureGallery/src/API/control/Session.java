/*
 * Created on 07.09.2004
 *
 * 
 */
package API.control;

import API.portal.model.RequestFrameSet;

//import java.security.InvalidKeyException;
//import java.security.NoSuchAlgorithmException;
//import java.security.NoSuchProviderException;
//import java.security.PublicKey;
//import java.security.SignatureException;
//import java.security.cert.Certificate;
//import java.security.cert.CertificateEncodingException;
//import java.security.cert.CertificateException;

/**
 * @author tobi
 *	
 *	ubernimmt das certifizieren des user- logins 
 */
public class Session {
	private String create_date;
	private long sessionID;
	private String usr;
	private RequestFrameSet rfs = null ; // hier wird der aktuelle Zustand des Framesets des Users gespeichert
	
	/**
	 * 
	 * @param arg0
	 */
	public Session(String usr) {
//		super(arg0);
	}
	
	/**
	 * @return Returns the create_date.
	 */
	public String getCreate_date() {
		return create_date;
	}

	/**
	 * @param create_date The create_date to set.
	 */
	public void setCreate_date(String create_date) {
		// TODO automatisch den aktuellen Zeitpunkt eintragen
		this.create_date = create_date;
	}
	
	/**
	 * @return Returns the sessionID.
	 */
	public long getSessionID() {
		return sessionID;
	}
	
	/**
	 * @param sessionID The sessionID to set.
	 */
	private void setSessionID(long sessionID) {
		this.sessionID = sessionID;
	}
	
	/**
	 * @return Returns the usr.
	 */
	public String getUsr() {
		return usr;
	}
	
	/**
	 * @param usr
	 */
	public void setUsr(String usr) {
		this.usr = usr;
	}
	
	/* (non-Javadoc)
	 * @see java.lang.Object#toString()
	 */
	public String toString() {
		return new String("Certificat des users: "+
				usr +", erstellungsdatum: " + 
				create_date +", SessionID: "+sessionID);
	}
}