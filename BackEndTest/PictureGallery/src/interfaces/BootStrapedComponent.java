package interfaces;


/**
 * Interface für eine startbare Komponente, die eine <code>init</code> Methode für
 * die Weitergabe der Properties implemtieren muss.
 * @author danny, tobi, franky
 * @since 05.05.04
 */
public interface BootStrapedComponent{

	/**
	 * Initialisiert ueber eine xml Datei die 
	 * gewuenschte Komponente
	 */
	public void init(String jconfigserverUrl);
	public void init(String jconfigUrl, String loadClass);
}
