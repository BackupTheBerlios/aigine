
import interfaces.BootStrapedComponent;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URL;
import java.rmi.server.RMIClassLoader;

/**
 * Laedt eine Klasse vom Server und startet sie.
 * Liest die zur Verfgung stehenden Komponenten aus einem
 * Repository und bietet eine Konfigurationsoberflche fr
 * die RMI Kommunikationseinstellungen an.
 * @author danny
 * @since 05.05.2004
 * @modified Franky
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
	public URL url;


	
	/**
	 * url vom ClassServer.
	 */
	private static String url_classServer;
	
	/**
	 * url vom jconfigserver.
	 */
	private static String url_JconfigServer;

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
	public RMIComponentLoader(String args[])
		throws
			MalformedURLException,
			ClassNotFoundException,
			InstantiationException,
			IllegalAccessException,
			FileNotFoundException,
			IOException {
		System.out.println("=> RMIComponentLoader()");
		
		//		setzen der server Adressen
		setClassServerFromArgs(args);
		setJconfigServerFromArgs(args);
		// uebergebenen URL lesen und Komponente laden vom Classserver laden
		url = new URL(url_classServer);		
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
		component.init();
	}

	
	private void setClassServerFromArgs(String[] args) {
		String c_url = getArgument(args,"-classServer");
		if ( c_url != null ) {
			url_classServer = c_url;
		}
		else {
			//documentRoot = System.getProperty("java.io.tmpdir");
			url_classServer = "http://localhost:2002/";
		}
	}
    
	private void setJconfigServerFromArgs(String[] args) {
		String c_url = getArgument(args,"-jconfigServer");
		if ( c_url != null ) {
			try {
				url_JconfigServer = c_url;
				//port = Integer.parseInt(myPort);
			}
			catch (Exception e) {
               }
		}
	}
    /**
     * holt uebergebenes Parameter aus Argument Array . 
     * In der Form "-param Argument"
     * @param args	aus main
     * @param param	welchen parameter 
     * @return Parameter
     */
    private String getArgument(String[] args,String param) {
		String ret = null;
		for ( int i = 0; i < args.length;i++) {
			if ( args[i].equals(param)) {
				if ( (i+1) <= args.length ) {
					ret = args[i+1];
				}
			}
		}
		return ret;
	}
	
	

	/**
	 * @param PropertyFileName
	 */
	// TODO Prfung der Parameter und Help Aufruf
	// TODO Parameter fr GUI oder Shell Modus
	public static void main(String args[]) {
		if (args.length < 2) {
			System.out.println(
				"Usage: java RMIComponentLoader ClassServerURL [e.g. http://localhost:2002/   ...]");
			return;
		}
		// SecurityManager setzen und RMIComponentLoader starten.
		System.setSecurityManager(new RMIComponentBootstrapSecurityManager());
		try {
			rcl = new RMIComponentLoader(args);
			
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
