/*
 * Created on 08.08.2004
 * File: package exceptions;.HTTPException.java
 */
package exceptions;

import java.io.IOException;

/**
 * An HTTPException signals a run-time failure while processing HTTP requests and responses.
 * @author danny, KOBAYASHI Masatomo
 * @since 08.08.2004 16:11:54
 * @version 0.02 Umbau auf Bootstraped Komponente
 * @version 0.01 http://www-ui.is.s.u-tokyo.ac.jp/~kobayash/misc/ui/util/SimpleWebServer.java
 */

public class HTTPException extends IOException {
	/**
	 * Contains the status code.
	 */
	public final String statusCode;

	/**
	 * Contains the reason phrase.
	 */
	public final String reasonPhrase;

	/**
	 * Contains an array of { the method, the request-URI, the HTTP version }.
	 */
	public final String[] requestLine;
		
	/**
	 * Notifies an HTTP error.
	 * 
	 * @param status the status code
	 * @param reason the reason phrase
	 * @param request the request line that caused this error
	 */
	public HTTPException(String status, String reason, String[] request) {
		statusCode = status;
		reasonPhrase = reason;
		requestLine = request;
	}
}
	
