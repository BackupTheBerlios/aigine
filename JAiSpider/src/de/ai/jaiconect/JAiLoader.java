/*
 * Created on 06.10.2004
 *
 */
package de.ai.jaiconect;

import java.io.File;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLClassLoader;

/**
 * Startet einen URLClassLoader und lädt damit die angebene
 * Starterbibliothek.
 * 
 * 
 * @author danny
 *
 */
public class JAiLoader {
	
	protected static String NAME_CLIENT_LIB = "JAiNode.jar";
	protected static String NAME_CLIENT_MAIN = "JAiConect.JAiNode";
	protected static String manager;
	/**
	 * 
	 */
	public JAiLoader() {
		try {
			URL urlList[] = {new File(manager + NAME_CLIENT_LIB).toURL()};
			URLClassLoader loader = new URLClassLoader(urlList);
			Class c = loader.loadClass(NAME_CLIENT_MAIN);
			// Klasseninstanzierung
			Object juppsLoader = c.newInstance();
			
		} catch (MalformedURLException e) {
			e.printStackTrace();
			System.out.println("Keine gültige URL übergeben:\n" + e.getLocalizedMessage());
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
			System.out.println("Klasse " + NAME_CLIENT_MAIN + " konnte nicht geladen werden:\n" + e.getLocalizedMessage());
		} catch (InstantiationException e) {
			e.printStackTrace();
			System.out.println("Klasse " + NAME_CLIENT_MAIN + " konnte nicht instanziert werden:\n" + e.getLocalizedMessage());
		} catch (IllegalAccessException e) {
			e.printStackTrace();
			System.out.println("Zugriff für Klasseninstanzierung " + NAME_CLIENT_MAIN + " nicht möglich:\n" + e.getLocalizedMessage());				
		}		
	}
	
	public static void main(String args[]) {
		System.out.println("usage: jeupps.JAiLoader http://ManagerServerName.domain:Portnumber");
		if(args[0] != null){
			manager = args[0];
			new JAiLoader();
		}
	}
}
