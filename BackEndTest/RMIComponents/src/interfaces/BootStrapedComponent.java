package interfaces;
import java.util.Properties;

/**
 * Interface f�r eine startbare Komponente, die eine <code>init</code> Methode f�r
 * die Weitergabe der Properties implemtieren muss.
 * @author danny, tobi
 * @since 05.05.04
 */
public interface BootStrapedComponent{

	/**
	 * Initialisiert ueber eine Property Datei die 
	 * gewuenschte Komponente
	 */
	public void init(Properties p);
}
