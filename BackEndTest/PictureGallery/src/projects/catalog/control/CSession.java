/*
 * Created on 07.09.2004
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package projects.catalog.control;

import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;
import java.security.NoSuchProviderException;
import java.security.PublicKey;
import java.security.SignatureException;
import java.security.cert.Certificate;
import java.security.cert.CertificateEncodingException;
import java.security.cert.CertificateException;

/**
 * @author your mama
 *	
 *	ubernimmt das certifizieren des user- logins 
 */
public class CSession extends Certificate {

	
	private String create_date;
	private long sessionID;
	private String usr;
	
	
	
	
	/**
	 * 
	 * @param arg0
	 */
	public CSession(String arg0) {
		super(arg0);
		// TODO Auto-generated constructor stub
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
	public void setSessionID(long sessionID) {
		this.sessionID = sessionID;
	}
	/**
	 * @return Returns the usr.
	 */
	public String getUsr() {
		return usr;
	}
	/**
	 * @param usr The usr to set.
	 */
	public void setUsr(String usr) {
		this.usr = usr;
	}
	    

	
	
	
	

	/* (non-Javadoc)
	 * @see java.security.cert.Certificate#getEncoded()
	 */
	public byte[] getEncoded() throws CertificateEncodingException {
		// TODO Auto-generated method stub
		return null;
	}

	/* (non-Javadoc)
	 * @see java.lang.Object#toString()
	 */
	public String toString() {
		
		return new String("Certificat des users: "+
				usr +", erstellungsdatum: " + 
				create_date +", SessionID: "+sessionID);
	}

	/* (non-Javadoc)
	 * @see java.security.cert.Certificate#getPublicKey()
	 */
	public PublicKey getPublicKey() {
		
		return null;
	}

	/* (non-Javadoc)
	 * @see java.security.cert.Certificate#verify(java.security.PublicKey)
	 */
	public void verify(PublicKey arg0) throws CertificateException,
			NoSuchAlgorithmException, InvalidKeyException,
			NoSuchProviderException, SignatureException {
		// TODO Auto-generated method stub

	}

	/* (non-Javadoc)
	 * @see java.security.cert.Certificate#verify(java.security.PublicKey, java.lang.String)
	 */
	public void verify(PublicKey arg0, String arg1)
			throws CertificateException, NoSuchAlgorithmException,
			InvalidKeyException, NoSuchProviderException, SignatureException {
		// TODO Auto-generated method stub
		

	}

}
