// Copyright MageLang Institute; Version $Id: ClassFileServer.java,v 1.9 2005/01/29 15:57:20 danny Exp $
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
 * org comment:
 * The ClassFileServer implements a ClassServer that reads class files from the
 * file system. See the doc for the "Main" method for how to run this server.
 * package examples.classServer;
 */

import java.io.*;



/**
 * Startet einen HTTP Server, um das Repository für den Klassenlader
 * anzubieten und einen JConfigServer für die angebotenen Komponenten
 * 
 * @author danny
 * @since Jan 29, 2005 3:48:04 PM
 *
 */

public class ClassFileServer extends ClassServer {

	private String repository = new String("bin");

	/**
	 * Constructs a ClassFileServer.
	 * 
	 * @param classpath
	 *            the classpath where the server locates classes
	 */
	public ClassFileServer(int port, String repository) throws IOException {
		super(port);
		System.out.println("=> ClassFileServer(" + port + ", " + repository + ")");
		this.repository = repository;
		System.out.println("<= ClassFileServer");
	}

	/**
	 * Returns an array of bytes containing the bytecodes for the class
	 * represented by the argument <b>path </b>. The <b>path </b> is a dot
	 * separated class name with the ".class" extension removed.
	 * 
	 * @return the bytecodes for the class
	 * @exception ClassNotFoundException
	 *                if the class corresponding to <b>path </b> could not be
	 *                loaded.
	 */
	public byte[] getBytes(String path, String fileExt) throws IOException,
			ClassNotFoundException {
		byte[] bytecodes = null;
		File f = null;
		FileInputStream fin = null;
		DataInputStream in = null;
		System.out.println("searching in repository: " + repository
				+ " for file:" + path + fileExt);
		f = new File(repository + File.separator
				+ path.replace('.', File.separatorChar) + fileExt);
		System.out.println("reading in repository: " + repository + " file:"
				+ path + fileExt);
		f = new File(repository + File.separator
				+ path.replace('.', File.separatorChar) + fileExt);
		super.bytesCont += f.length();
		fin = new FileInputStream(f);
		in = new DataInputStream(fin);
		bytecodes = new byte[(int) f.length()];
		in.readFully(bytecodes);
		return bytecodes;
	}


	/**
	 * @return Returns the bytesCont.
	 */
	public long getBytesCont() {
		return bytesCont;
	}

}