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
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public class CSession extends Certificate {

	/**
	 * @param arg0
	 */
	public CSession(String arg0) {
		super(arg0);
		// TODO Auto-generated constructor stub
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
		// TODO Auto-generated method stub
		return null;
	}

	/* (non-Javadoc)
	 * @see java.security.cert.Certificate#getPublicKey()
	 */
	public PublicKey getPublicKey() {
		// TODO Auto-generated method stub
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
