package newdefault;

import java.io.File;
import java.io.IOException;

import org.jconfig.Configuration;
import org.jconfig.ConfigurationManager;
import org.jconfig.server.ConfigurationServer;

/*
 * filename: RMIRepositoryServer.java
 * project: ClassServer
 * Created on Jan 29, 2005 4:04:38 PM
 *
 */

/**
 * Verwaltet einen ClassFile- und einen JConfigServer 
 * 
 * @author danny
 * @since Jan 29, 2005 4:04:38 PM
 *
 */
public class RMIRepositoryServer {

	private int jConfigServerPort = 2201;
	private int classServerPort = 2202;
	private String jConfigServerRepository = new String("config");
	private String classServerRepository = new String("bin");

	private ClassFileServer classServer = null;
	
	private static final Configuration configuration = ConfigurationManager
			.getConfiguration("server");

	/**
	 */
	public RMIRepositoryServer() {
		System.out.println("=> RMIRepositoryServer");
		// Parameter lesen
		this.classServerPort = configuration.getIntProperty("port",
				classServerPort, "classserver");
		this.classServerRepository = configuration.getProperty("repository",
				classServerRepository, "classserver");
		this.getRepositoryContent(this.classServerRepository);
		this.jConfigServerPort = configuration.getIntProperty("port",
				jConfigServerPort, "jconfigserver");
		this.jConfigServerRepository = configuration.getProperty("repository",
				jConfigServerRepository, "jconfigserver");
		this.getRepositoryContent(this.jConfigServerRepository);
		// Serverkomponenten starten
		try {
			classServer = new ClassFileServer(classServerPort, classServerRepository);
		} catch (IOException e) {
			e.printStackTrace();
			System.err
					.println("Unable to start ClassServer: " + e.getMessage());
		}
		System.out.println("=> JConfigServer(" + jConfigServerPort + ", " + jConfigServerRepository + ")");
		ConfigurationServer cs = new ConfigurationServer(jConfigServerPort, jConfigServerRepository);
		System.out.println("<= JConfigServer");
		System.out.println("<= RMIRepositoryServer");
	}

	/**
	 * Gibt eine Zusammenfassung des Ordnerinhaltes aus.
	 * @param path
	 */
	// TODO mit RuntimeReflection nach gültigen RMI Komponenten prüfen und 
	// entsprechende config für das Repository generieren bzw. validieren
	private void getRepositoryContent(final String path) {
		File file = new File(path);
		File[] dirContent = file.listFiles();
		int fileCounter = 0;
		int dirCounter = 0;
		for (int i = 0; dirContent != null && i < dirContent.length; i++) {
			File tmpFile = dirContent[i];
			if (tmpFile.isDirectory()) {
				dirCounter++;
			} else {
				fileCounter++;
			}
		}
		System.out.println("repository: " + path + " => " + fileCounter
				+ " files, " + dirCounter + " directories.");
	}
	
	public static void main(String args[]) {
		RMIRepositoryServer rrs = new RMIRepositoryServer();
	}
}