/*
 * Created on 05.05.2004
 *
 */
package interfaces;
import java.util.Properties;

/**
 * Interface f\x{00FC}r eine Komponente.
 * @author danny, tobi
 * @since 27.07.04
 */
public interface BootStrapedComponent{

	/**
	 * Initialisiert ueber eine Property Datei die 
	 * gewuenschte Componete
	 * @throws RemoteException
	 */
	public void init(Properties p);
}
