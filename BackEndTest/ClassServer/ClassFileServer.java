// Copyright MageLang Institute; Version $Id: ClassFileServer.java,v 1.7 2004/10/03 14:59:37 mr_nice Exp $

/*
 * Copyright (c) 1996, 1996, 1997 Sun Microsystems, Inc. All Rights Reserved.
 *
 * SUN MAKES NO REPRESENTATIONS OR WARRANTIES ABOUT THE SUITABILITY OF THE
 * SOFTWARE, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE, OR NON-INFRINGEMENT. SUN SHALL NOT BE LIABLE FOR ANY DAMAGES
 * SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING OR DISTRIBUTING
 * THIS SOFTWARE OR ITS DERIVATIVES.
 */
//package examples.classServer;

import java.io.*;
import java.util.jar.JarFile;

import org.jconfig.server.ConfigurationServer;
import org.jconfig.Configuration;
import org.jconfig.ConfigurationManager;
/**
 * The ClassFileServer implements a ClassServer that reads class files from the
 * file system. See the doc for the "Main" method for how to run this server.
 */


public class ClassFileServer extends ClassServer {
	private String repository;

	private String externalrepository;
	
	private static int DefaultServerPort = 2001;
	
	private static final Configuration configuration =
		 ConfigurationManager.getConfiguration("config/server");
	//TODO FOSION alles in die main config schmeissen
		 
	private static int port = configuration.getIntProperty("port",2002,"classserver");

	/**
	 * Constructs a ClassFileServer.
	 * 
	 * @param classpath
	 *            the classpath where the server locates classes
	 */
	public ClassFileServer() throws IOException {
		
		super(port);
				
		this.repository = configuration.getProperty("repository",System.getProperty("java.io.tmpdir"),"classserver");
		
		this.getRepositoryContent(repository);
		
		this.externalrepository = configuration.getProperty("externalrepository",System.getProperty("java.io.tmpdir"),"classserver");
		System.out.println("CLassFileServer Started whith\n classpath = " + this.repository
										+ " | on port = " + port + " | and the externalDir = "
										+ this.externalrepository);
		//Reisst einen Jconfig Server hoch
		// port = 
		System.out.println("\nJetzt starte ich Config Server...");
		ConfigurationServer cs = new ConfigurationServer(configuration.getIntProperty("port",2010,"jconfigserver"),configuration.getProperty("repository","","jconfigserver"));
		System.out.println("Done");
		
	}
	
	private void getRepositoryContent(String path) {
	File file = new File(path);
    
    File[] dirContent = file.listFiles();
    
    int anzahlDateien = 0;
    int anzahlVerzeichnisse = 0;
    
    for (int i = 0; i < dirContent.length; i++) {
        
        File tmpFile = dirContent[i];
        
        if(tmpFile.isDirectory()){
            
            anzahlVerzeichnisse++;
            
            
        }else{
            
            anzahlDateien++;
            
        }
        
    }
    
    System.out.println("Im Verzeichnis " + path
        + " befinden sich " + anzahlDateien
        + " Dateien und " + anzahlVerzeichnisse
        + " Unterverzeichnisse." ); 
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
		JarFile	jf = null;
		System.out.println("reading at classpath: " + repository + " file:"
				+ path + fileExt);
		if(fileExt == ".jar") {
		 jf= new JarFile(new File(repository + File.separator
					+ path.replace('.', File.separatorChar) + fileExt+"!/"));
		} //TODO vieleicht im den jar:http... path bereitz uebergeben als classpath...
		f = new File(repository + File.separator
				+ path.replace('.', File.separatorChar) + fileExt);
		int length = (int) (f.length());
		super.bytesCont += length;
		//System.out.println("laenge des bytes:" + length);

		if (length == 0) {
			System.out.println("reading at exteralpath: " + externalrepository
					+ " file:" + path + fileExt);
			f = new File(externalrepository + File.separator
					+ path.replace('.', File.separatorChar) + fileExt);
			length = (int) (f.length());
			super.bytesCont += length;
			//System.out.println("laenge des bytes:" + length);

		}
		if (length == 0) {
			System.out.println("Zero length file: " + path);
			throw new IOException("File length is zero: " + path);

		} else {
			//System.out.println("else:::");
			FileInputStream fin = new FileInputStream(f);
			//System.out.println("new inputFileStream: "+fin);
			DataInputStream in = new DataInputStream(fin);
			//System.out.println("new DataInputStream: "+fin);	
			bytecodes = new byte[length];
			in.readFully(bytecodes);

			if (fileExt.equals(".hbm.xml")) {
				System.out.println("\nEine " + fileExt + " gelesen!!\n");
				System.out.println(new String(bytecodes));
			}

		}
		return bytecodes;
	}

	/**
	 * Main method to create the class server that reads class files. This takes
	 * two command line arguments, the port on which the server accepts requests
	 * and the root of the classpath. To start up the server: <br>
	 * <br>
	 * 
	 * <code>   java ClassFileServer <port> <classpath>
	 * </code><br>
	 * <br>
	 * 
	 * The codebase of an RMI server using this webserver would simply contain a
	 * URL with the host and port of the web server (if the webserver's
	 * classpath is the same as the RMI server's classpath): <br>
	 * <br>
	 * 
	 * <code>   java -Djava.rmi.server.codebase=http://zaphod:2001/ RMIServer
	 * </code>
	 * <br>
	 * <br>
	 * 
	 * You can create your own class server inside your RMI server application
	 * instead of running one separately. In your server main simply create a
	 * ClassFileServer: <br>
	 * <br>
	 * 
	 * <code>   new ClassFileServer(port, classpath);
	 * </code>
	 */
	public static void main(String args[]) {
		
		
			try {
				new ClassFileServer();
				
			} catch (IOException e) {
				System.out.println("Unable to start ClassServer: "
						+ e.getMessage());
				e.printStackTrace();
			}

	}
	

	/**
	 * @return Returns the bytesCont.
	 */
	public int getBytesCont() {
		return bytesCont;
	}
	

	
	
	
}