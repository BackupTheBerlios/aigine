
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URL;
import java.rmi.server.RMIClassLoader;
import java.util.Properties;

import interfaces.BootStrapedComponent;

/**
 * Laedt eine Klasse vom Server und startet sie.
 * Liest die zur Verfgung stehenden Komponenten aus einem
 * Repository und bietet eine Konfigurationsoberflche fr
 * die RMI Kommunikationseinstellungen an.
 * @author danny
 * @since 05.05.2004
 *
 * @version 0.90
 * - Umstellung auf eine zu uebergebene Property Datei, die alle Informationen
 *   fuer die RMI Kommunikation und die von der jeweiligen Komponente 
 *   benoetigten Einstellungen enthaelt.
 * - Verwendung eines Interfaces fuer den einheitlichen Zugriff auf Komponenten
 * @version 0.02
 * - kleine Erweiterung der Properties
 * @version 0.01
 * Einsatz fr die Uebung VSYS:
 * - verwendet die Java System Properties fr die Konfigurationseinstellungen.
 * - startet die geladene Klasse (verwendet Thread.run() Aufruf)
 */

public class RMIComponentLoader {

	/**
	 * Reference to ourselves
	 */
	private static RMIComponentLoader rcl;

	/**
	* Speichert das gelesene Property "codebase" fuer
	* den Zugriff auf den Klassenserver
	* @see java.rmi.server.RMIClassLoader
	 */
	private URL url;

	/**
	 * Speicherung der gelesenen Properties fuer die
	* Uebergabe an den Komponenten Thread.
	 */
	private Properties p;	
	
	/**
	 * Name der zu verwendenen Property Datei.
	 */
	private static String PROPERTY_NAME;

	/**
	 * Name der zu ladenen Komponente.
	 */
	private static String compClassName;

	/**
	 * Klasse fuer die zu ladene Komponente.
	 */
	private Class compClass;

	/**
	 * Zugriff auf den geladenen Komponenten Thread.
	 */
	private BootStrapedComponent component;

	/**
	 * Konstruktor
	 */
	// TODO Verwendung einer Konfigurationsoberflche fr die Einstellung aller bentigten Parameter.
	public RMIComponentLoader()
		throws
			MalformedURLException,
			ClassNotFoundException,
			InstantiationException,
			IllegalAccessException,
			FileNotFoundException,
			IOException {
		System.out.println("=> RMIComponentLoader()");
		
		// Properties einlesen
		p = new Properties();
		FileInputStream fin = new FileInputStream(PROPERTY_NAME);			
		p.load(fin);
		System.out.println("geladene Properties:\n" + p);		
		// URL lesen und Komponente laden
		url = new URL(p.getProperty("codebase"));		
		compClassName = "API.control.BootStrapedApplicationStarter";
		
		System.out.println("Asking for: " + url + " and " + compClassName);
		compClass = RMIClassLoader.loadClass(url, compClassName);
		System.out.println("After loading Component Class => " + compClass);

		try {
			component = (BootStrapedComponent) compClass.newInstance();	
		} catch (Exception e) {
			e.printStackTrace();
			System.out.println("Fehler: " + e.getMessage());
		}
		

		// Komponente initialisieren und starten
		//component.run();
		component.init(p);
	}

	/**
	 * @param PropertyFileName
	 */
	// TODO Prfung der Parameter und Help Aufruf
	// TODO Parameter fr GUI oder Shell Modus
	public static void main(String args[]) {
		if (args.length < 1) {
			System.out.println(
				"Usage: java RMIComponentLoader PropertyFileName.prop [propName02.prop, propName03.prop ...]");
			return;
		}
		
		PROPERTY_NAME = args[0];
		
		// SecurityManager setzen und RMIComponentLoader starten.
		System.setSecurityManager(new RMIComponentBootstrapSecurityManager());
		try {
			rcl = new RMIComponentLoader();
		} catch (MalformedURLException e) {
			e.printStackTrace();
			System.out.println(
				"URL not specified correctly for the Component class: "
					+ e.getMessage());
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
			System.out.println(
				"RMIComponentLoader, class not found: " + e.getMessage());
		} catch (InstantiationException e) {
			e.printStackTrace();
			System.out.println(
				"RMIComponentLoader, class could not be instantiated"
					+ e.getMessage());
		} catch (IllegalAccessException e) {
			e.printStackTrace();
			System.out.println("Internal error" + e.getMessage());
		} catch (FileNotFoundException e) {
			e.printStackTrace();
			System.out.println(
				"RMIComponentLoader, no property file found " + e.getMessage());
		} catch (IOException ioe) {
			System.out.println(
				"RMIComponentLoader, couldn't read properties"
					+ ioe.getMessage());
		}
	}
}
