import interfaces.BootStrapedComponent;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URL;
import java.rmi.server.RMIClassLoader;

import exception.ParameterNotFoundException;

/**
 * Lädt den ApplicationStarter vom ClassServer und startet ihn.
 * @author danny
 * @since 05.05.2004
 *
 * @version 0.95
 * - wird nur noch für das Laden und Starten des ApplicationStarter verwendet.
 * @version 0.90
 * - Umstellung auf eine zu uebergebene Property Datei, die alle Informationen
 *   fuer die RMI Kommunikation und die von der jeweiligen Komponente 
 *   benoetigten Einstellungen enthaelt.
 * - Verwendung eines Interfaces fuer den einheitlichen Zugriff auf Komponenten
 * @version 0.02
 * - kleine Erweiterung der Properties
 * @version 0.01
 * Einsatz für die Uebung VSYS:
 * - verwendet die Java System Properties für die Konfigurationseinstellungen.
 * - startet die geladene Klasse (verwendet Thread.run() Aufruf)
 */

public class RMIComponentLoader {

	/**
	 * Reference to ourselves
	 */
	private static RMIComponentLoader ownReference;

	/**
	 * url vom ClassServer.
	 */
	private static String urlClassServer;

	/**
	 * url vom jconfigserver.
	 */
	private static String urlJconfigServer;

	/**
	 * Name der Starterklasse (lädt die angeforderte Klasse).
	 */
	private static String compClassStarterName = "API.control.BootStrapedApplicationStarter";

	/**
	 * Klasse fuer die zu ladene Komponente.
	 */
	private Class compClassStarterClass;

	/**
	 * Zugriff auf den geladenen Komponenten Thread.
	 */
	private BootStrapedComponent compClassStarter;

	/**
	 * Konstruktor, prüft Argumente und lädt, instanziert und initialisiert
	 * den Applikationstarter.
	 */
	// TODO Verwendung einer Konfigurationsoberfläche für die Einstellung aller 
	//      möglichen Parameter.
	public RMIComponentLoader(String args[]) throws MalformedURLException,
			ClassNotFoundException, InstantiationException,
			IllegalAccessException, FileNotFoundException, IOException,
			ParameterNotFoundException {
		URL url = null;
		String loadClass = null;
		System.out.println("=> RMIComponentLoader()");
		// setzen der Server Adressen
		setClassServerFromArgs(args);
		setJconfigServerFromArgs(args);
		url = new URL(urlClassServer);
		// Starter laden
		System.out.println("Asking for: " + compClassStarterName + " @ " + url);
		compClassStarterClass = RMIClassLoader.loadClass(url,
				compClassStarterName);
		System.out.println("Class loaded => " + compClassStarterClass);
		// neue Instanz des Startes erzeugen
		compClassStarter = (BootStrapedComponent) compClassStarterClass
				.newInstance();
		// Komponente initialisieren und starten
		try {
			loadClass = getParameter(args, "-loadClass");
			compClassStarter.init(urlJconfigServer, loadClass);
		} catch (ParameterNotFoundException pnfe) {
			// Keine Klasse angegeben
			compClassStarter.init(urlJconfigServer);
		}
	}

	/**
	 * Setzen der KlassenServer Variablen (parst die Argumente nach -classServer)
	 * @param java program args
	 */
	private void setClassServerFromArgs(String[] args)
			throws ParameterNotFoundException {
		urlClassServer = getParameter(args, "-classServer");
		System.out.println("Set ClassServer URL: " + urlClassServer);
	}

	/**
	 * Setzen der KonfigurationsServer Variablen (parst die Argumente nach -jconfigServer)
	 * @param java program args
	 */
	private void setJconfigServerFromArgs(String[] args)
			throws ParameterNotFoundException {
		urlJconfigServer = getParameter(args, "-jconfigServer");
		System.out.println("Set JConfigServer URL: " + urlJconfigServer);
	}

	/**
	 * holt uebergebenes Parameter aus Argument Array. 
	 * In der Form "-param Argument"
	 * @param args	aus main
	 * @param param	welchen parameter 
	 * @return Parameter
	 * @throws ParameterNotFoundException
	 */
	private String getParameter(String[] args, String param)
			throws ParameterNotFoundException {
		String ret = null;
		for (int i = 0; i < args.length; i++) {
			if (args[i].equals(param)) {
				if ((i + 1) <= args.length) {
					ret = args[i + 1];
					System.out.println("getParameter: param=" + param
							+ ", value=" + ret);
				}
			}
		}
		if (ret == null)
			throw new ParameterNotFoundException(param);

		return ret;
	}

	/**
	 */
	// TODO Parameter für die zu startene Klasse und 
	//      entsprechende init Methode im ApplikationStarter, z.B. loadClass
	// TODO Parameter für GUI oder Shell Modus
	public static void main(String args[]) {
		if (args.length < 4 || args[0].equals("-h") || args[0].equals("-help")
				|| args[0].equals("--help")) {
			System.out.println("Usage: java RMIComponentLoader "
					+ "-jconfigServer URLforJconfigServer "
					+ "-classServer URLforClassServer "
					+ "[-loadClass ClassName]");
			return;
		}
		// SecurityManager setzen und RMIComponentLoader starten.
		System.setSecurityManager(new RMIComponentBootstrapSecurityManager());
		try {
			ownReference = new RMIComponentLoader(args);
		} catch (MalformedURLException e) {
			System.err
					.println("Error: RMIComponentLoader, URL not specified correctly for the component class: "
							+ e.getMessage());
		} catch (ClassNotFoundException e) {
			System.err.println("Error: RMIComponentLoader, class not found: "
					+ e.getMessage());
		} catch (InstantiationException e) {
			System.err
					.println("Error: RMIComponentLoader, class could not be instantiated: "
							+ e.getMessage());
		} catch (IllegalAccessException e) {
			System.out.println("Error: RMIComponentLoader, internal error: " + e.getMessage());
			e.printStackTrace();			
		} catch (FileNotFoundException e) {
			System.err.println("Error: RMIComponentLoader, file not found: "
					+ e.getMessage());
		} catch (IOException ioe) {
			System.err.println("Error: RMIComponentLoader, io error: "
					+ ioe.getMessage());
		} catch (ParameterNotFoundException pnfe) {
			System.err.println("Error: RMIComponentLoader, couldn't read parameter: "
					+ pnfe.getMessage());
		}
	}
}