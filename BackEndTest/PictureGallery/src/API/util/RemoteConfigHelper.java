package API.util;
import org.jconfig.*;
import org.jconfig.handler.*;
import org.jconfig.ConfigurationManager;
/**
 * @author Frank Otto
 *	dient als Helper zur Jconfigserver Kommunikation 
 */
public class RemoteConfigHelper {

	private static ConfigurationManager cm = ConfigurationManager.getInstance();

	/**
	 * holt eine Configuration vom Configserver und speichert diese
	 * in dem configurationsmanager cm holen mit
	 * ConfigurationManager.getConfiguration("configname");
	 * WICHTIG:  im uebrgeben url darf muss http:// vorkommen
	 * @param cservurl wo ist der config server http://localhost:2001/ 
	 * @param configname Name der Configurationsdatei ohne .xml  !
	 * @exception	ConfigurationManagerException wenn er keine bekommt dann meckert er
	 */
	public static void load(String cservurl, String configname) {
		//jconfig serverhandle instanzieren...
		ConfigServerHandler handler = new ConfigServerHandler();
		String tmp = new String("http://");
		try {

			//String zusammenbauen in der form jconfig://www.der-server.de/configname
			//hierzu wird wenn vorhanden das "http://" rausgesaebelt
			
			if (cservurl.substring(0, 7).compareTo(tmp) == 0) {
				tmp = cservurl.substring(7);

			} else {
				tmp = cservurl;
			}

			String url = new String("jconfig://" + tmp + configname);
			System.out.println(url);
			handler.setURL(url);
			cm.load(handler, configname);

		} catch (ConfigurationManagerException cme) {
			// do something here since we did not get a configuration
			System.out
					.println("getting no config from: "
							+ cservurl
							+ "\nCheck jconfigserver output \n path and config name!!!");
			cme.printStackTrace();
		}

	}
	public static void store(String cservurl, Configuration config) {
		ConfigServerHandler handler = new ConfigServerHandler();
		//		  URLHandler urlHandler = new URLHandler();
		try {
			handler.setURL(cservurl);
			//			urlHandler.load(configname);
			//			ConfigurationManager cm = ConfigurationManager.getInstance();
			// we use the ConfigurationManager so it can store the config
			cm.save(handler, config);
			System.out.println("Stored on ");

		} catch (ConfigurationManagerException cme) {
			// do something here since we did not get a configuration
			System.out
					.println("can´t store on: "
							+ cservurl
							+ "\nCheck jconfigserver output, \n path and ConfigurationManager!!!");
			cme.printStackTrace();
		}

	}
	public ConfigurationManager getCM() {
		return cm;
	}

}

