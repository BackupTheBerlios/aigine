package API.util;
import org.jconfig.Configuration;

import API.model.RemoteObject;
/**
*
* Utility Klasse welche aus uebergebener 
* JConfig Configuration+gewuenschter category 
* alle Einstellungen in ein remoteObject packt und dieses
* zurueck gibt 
* @author Frank Otto s0503712
*/
public class RemoteObjectHelper {

	private RemoteObject ro;
	
  /**
   * Instanziert ein remoteObject und fuettert es mit den Konfigurationsdaten
   * -->trennung model/util
	 * @param configuration JConfig Configuration
	 * @param category Kategorie der _config.xml Datei 
	 */
  
  //TODO  XML Properties Schemaprüfung
	public RemoteObjectHelper(Configuration configuration, String category) {

		ro = new RemoteObject();
		System.out.println("=> RemoteObjectProperties()");

		ro.setDescription(configuration.getProperty("description", "", category));
		ro.setServicetyp(configuration.getProperty("typ", "", category));
		ro.setCodebase(configuration.getProperty("codebase", "", category));
		ro.setCompClassName(configuration.getProperty("compClassName", "", category));
		ro.setCompName(configuration.getProperty("compName", "", category));
		ro.setPort(configuration.getProperty("port", "", category));
		ro.setHostname(configuration.getProperty("hostname", "", category));
		ro.setAuthTyp(configuration.getProperty("authTyp", "", category));
		ro.setUsername(configuration.getProperty("user", "", category));
		ro.setPasswd(configuration.getProperty("passwd", "", category));
		ro.setRmiName("rmi://" + ro.getHostname() + ":" + ro.getPort() + "/");
		
		if(category.equals("vtlogicserver")){
			System.out.println("Category: "+ category );	
			ro.setManager(configuration.getProperty("manager","",category));
			ro.setManagerName(configuration.getProperty("managerName","",category));
		}
		
    System.out.println("<= RemoteObjectProperties() > " + this);

	}
  /**
   * 
	 * @return Konfiguriertes RemoteObject
	 */
	public RemoteObject getConfiguratedRemoteObject(){
    return ro;
  }


}
