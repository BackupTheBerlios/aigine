package interfaces;
import java.util.Properties;

/**
 * Interface für eine startbare Komponente, die eine <code>init</code> Methode für
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
