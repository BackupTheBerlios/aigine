// Copyright MageLang Institute; Version $Id: ClassServer.java,v 1.6 2005/01/29 15:57:20 danny Exp $

/*
 * Copyright (c) 1996, 1996, 1997 Sun Microsystems, Inc. All Rights Reserved.
 *
 * SUN MAKES NO REPRESENTATIONS OR WARRANTIES ABOUT THE SUITABILITY OF THE
 * SOFTWARE, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE, OR NON-INFRINGEMENT. SUN SHALL NOT BE LIABLE FOR ANY DAMAGES
 * SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING OR DISTRIBUTING
 * THIS SOFTWARE OR ITS DERIVATIVES.
 *
 * CopyrightVersion 1.1_beta
 */

//package examples.classServer;

import java.io.*;
import java.net.*;


/**
 * ClassServer is an abstract class that provides the
 * basic functionality of a mini-webserver, specialized
 * to load class files only. A ClassServer must be extended
 * and the concrete subclass should define the <b>getBytes</b>
 * method which is responsible for retrieving the bytecodes
 * for a class.<p>
 *
 * The ClassServer creates a thread that listens on a socket
 * and accepts  HTTP GET requests. The HTTP response contains the
 * bytecodes for the class that requested in the GET header. <p>
 *
 * For loading remote classes, an RMI application can use a concrete
 * subclass of this server in place of an HTTP server. <p>
 *
 * @see ClassFileServer
 */
public abstract class ClassServer implements Runnable {

	private ServerSocket server = null;
	private int port;
	private static String fileExtension;
	private static String jarURL;
	protected long bytesCont = 0;

	/**
	 * Constructs a ClassServer that listens on <b>port</b> and
	 * obtains a class's bytecodes using the method <b>getBytes</b>.
	 *
	 * @param port the port number
	 * @exception IOException if the ClassServer could not listen
	 *            on <b>port</b>.
	 */
	protected ClassServer(int port) throws IOException {
		this.port = port;
		server = new ServerSocket(port);
		newListener();
		

		
	}

	/**
	 * Returns an array of bytes containing the bytecodes for
	 * the class represented by the argument <b>path</b>.
	 * The <b>path</b> is a dot separated class name with
	 * the ".class" extension removed.
	 *
	 * @return the bytecodes for the class
	 * @exception ClassNotFoundException if the class corresponding
	 * to <b>path</b> could not be loaded.
	 * @exception IOException if error occurs reading the class
	 */
	public abstract byte[] getBytes(String path, String fileExt)
		throws IOException, ClassNotFoundException;

	/**
	 * The "listen" thread that accepts a connection to the
	 * server, parses the header to obtain the class file name
	 * and sends back the bytecodes for the class (or error
	 * if the class is not found or the response was malformed).
	 */
	public void run() {
		Socket socket;

		// accept a connection
		try {
			socket = server.accept();
		} catch (IOException e) {
			System.out.println("Class Server died: " + e.getMessage());
			e.printStackTrace();
			return;
		}

		// create a new thread to accept the next connection
		newListener();

		try {
			//System.out.println("=> ClassServer.run() new Connection" );
			DataOutputStream out =
				new DataOutputStream(socket.getOutputStream());
			try {
				// get path to class file from header
				BufferedReader in =
					new BufferedReader(
						new InputStreamReader(socket.getInputStream()));
				String path = getPath(in);
				// retrieve bytecodes
				System.out.println("request for " + path + fileExtension);
				byte[] bytecodes = getBytes(path, fileExtension);
				// send bytecodes in response (assumes HTTP/1.0 or later)
				try {
					out.writeBytes("HTTP/1.0 200 OK\r\n");
					out.writeBytes(
						"Content-Length: " + bytecodes.length + "\r\n");
					out.writeBytes("Content-Type: application/java\r\n\r\n");
					out.write(bytecodes);
					out.flush();
				} catch (IOException ie) {
					return;
				}

			} catch (Exception e) {
				// write out error response
				out.writeBytes("HTTP/1.0 400 " + e.getMessage() + "\r\n");
				out.writeBytes("Content-Type: text/html\r\n\r\n");
				out.flush();
			}

		} catch (IOException ex) {
			// eat exception (could log error to log file, but
			// write out to stdout for now).
			System.out.println("error writing response: " + ex.getMessage());
			ex.printStackTrace();

		} finally {
			try {
				System.out.println("total byeds read: "+ this.bytesCont);
				socket.close();
			} catch (IOException e) {
			}
		}
	}

	/**
	 * Create a new thread to listen.
	 */
	private void newListener() {
		(new Thread(this)).start();
	}

	/**
	 * Returns the path to the class file obtained from
	 * parsing the HTML header.
	 */
	private static String getPath(BufferedReader in) throws IOException {
		
		String line = in.readLine();
		System.out.println(" \n\n----------------------------------------------------------------------------");
		System.out.println("Request: "+line);
		
		String path = "";
		//System.out.println("=> ClassServer.getPath(path = " + line + ")" );
		// extract class from GET line
		if (line.startsWith("GET /")) {
			line = line.substring(5, line.length() - 1).trim();

			int index0 = line.indexOf(".class ");	
			//edding .hbm.xml files to get hibernate configurations ---by mr_nice 		
			int index1 = line.indexOf(".hbm.xml");
			
			int index2 = line.indexOf(".jar");
			if (index0 != -1 ) {
				path = line.substring(0, index0).replace('/', '.');
				fileExtension = ".class";
			}
			if (index1 != -1) {
				path = line.substring(0, index1).replace('/', '.');
				fileExtension = ".hbm.xml";
			}
			if (index2 != -1) {
				path = line.substring(0, index2).replace('/', '.');
				fileExtension = ".jar";
			    jarURL = "jar:http://"+path+fileExtension+"!/";
			}
		}
		
		// eat the rest of header
		do {
			line = in.readLine();
			System.out.println(line);
		} while (
			(line.length() != 0)
				&& (line.charAt(0) != '\r')
				&& (line.charAt(0) != '\n'));
		System.out.println("<= ClassServer.getPath(path) > " + path);
		if (path.length() != 0) {
			System.out.println("<= ClassServer.getPath(path) > " + path);
			return path;
		} else {
			throw new IOException("Malformed Header");
		}
	}
}
