/*
 * Created on 05.05.2004
 *
 */
package interfaces;
import java.util.Properties;

/**
 * Interface f\x{00FC}r eine Komponente.
 * @author danny, tobi, franky
 * @since 27.07.04
 */
public interface BootStrapedComponent{

	/**
	 * Initialisiert ueber eine xml Datei die 
	 * gewuenschte Componente
	 * @throws RemoteException
	 */
	public void init();
}
