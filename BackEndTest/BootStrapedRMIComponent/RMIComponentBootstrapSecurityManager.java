import java.rmi.RMISecurityManager;
import java.security.Permission;
//Copyright MageLang Institute; Version $Id: RMIComponentBootstrapSecurityManager.java,v 1.3 2005/01/29 14:35:20 danny Exp $
// TODO prüfen und aktualisieren der benötigten Berechtigungen
/**
 * This class defines a security policy for RMI applications
 * that are bootstrap loaded from a server. The relaxation in
 * security provided by this class is the minimal amount that
 * is required to bootstrap load and run a RMI client application.
 *
 * The policy changes from RMISecurityManager are:
 *
 * Security Check                    This Policy  RMISecurityManager
 * ------------------------------   ------------  ------------------
 * Access to Thread Groups               YES              NO
 * Access to Threads                     YES              NO
 * Create Class Loader                   YES              NO
 * System Properties Access              YES              NO
 * Connections                           YES              NO
 *
 */

public class RMIComponentBootstrapSecurityManager extends RMISecurityManager {

	/**
	 * Loaded classes are allowed to create class loaders.
	 */
	public synchronized void checkCreateClassLoader() {
		// Provide null override
	}

	/**
	 * Connections to other machines are allowed
	 */
	public synchronized void checkConnect(String host, int port) {
		// Provide null override
	}

	/**
	 * Loaded classes are allowed to manipulate threads.
	 */
	public synchronized void checkAccess(Thread t) {
		// Provide null override
	}

	/**
	 * Loaded classes are allowed to manipulate thread groups.
	 */
	public synchronized void checkAccess(ThreadGroup g) {
		// Provide null override
	}

	/**
	 * Loaded classes are allowed to access the system properties list.
	 */
	public synchronized void checkPropertiesAccess() {
		// Provide null override
	}
	/** 
	 * @see java.lang.SecurityManager#checkExit(int)
	 */
	public void checkExit(int status) {
		// Provide null override 
	}

	/** 
	 * @see java.lang.SecurityManager#checkPermission(java.security.Permission)
	 */
	public void checkPermission(Permission perm) {
		// Provide null override
	}

}
