package API.control;

import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.UnsupportedEncodingException;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.URLDecoder;
import java.net.UnknownHostException;
import java.rmi.RemoteException;
import java.util.HashMap;
import java.util.Hashtable;

import API.control.web.HeadFrame;

import exceptions.HTTPException;

/**
 * A <code>WebServer</code> can process only HTTP GET/HEAD requests for some media types,
 * otherwise it returns an error response.
 * 
 * @author danny, KOBAYASHI Masatomo
 * @since 08.08.2004 16:11:54
 * @version 0.01 http://www-ui.is.s.u-tokyo.ac.jp/~kobayash/misc/ui/util/SimpleWebServer.java
 */
public abstract class WebServer implements Runnable {

    private ServerSocket server_socket;
    private HashMap mime_types;

	/**
	 * Für die Ausgabe des Headers.
	 */
	private HeadFrame header = new HeadFrame();

    private int port;
    private String root;

    /**
     * Starts a web server with the specified port and the specified document root.
     * Startet neuen <code>ServerSocket</code> und den <code>Thread</code>.
     * @param port the port # for the server socket
     * @param root the root path of the public documents
     * @throws IOException
     */
    protected void init(int port, String root) {
        this.port= port;
        this.root= root;

        try {
            server_socket= new ServerSocket(this.port);
        } catch (IOException e) {

            e.printStackTrace();
        }
        mime_types= new HashMap();

        registerMIMEType("html", "text/html");
        registerMIMEType("css", "text/css");
        registerMIMEType("js", "text/javascript");
        registerMIMEType("txt", "text/plain");
        registerMIMEType("class", "application/java");
        registerMIMEType("jar", "application/java-archive");
        registerMIMEType("gif", "image/gif");
        registerMIMEType("jpg", "image/jpeg");
        registerMIMEType("png", "image/png");

        new Thread(this).start();
        // create the initial thread to accept the first connection<
    }

    /**
     * Associates a MIME type with a file extension.
     * 
     * @param suffix the extension to be associated with the <code>type</code>.
     * @param type the mime type for registration
     */
    protected void registerMIMEType(String suffix, String type) {
        mime_types.put(suffix, type);
    }

    /**
     * The <i>listen</i> thread that accepts a connection to the server,
     * parses a request, and sends back a valid or error response.
     */
    public void run() {        
        Socket socket;

        try {
            socket= server_socket.accept(); // accept a connection
        } catch (IOException e) {
            e.printStackTrace();
            return;
        }

        new Thread(this).start();
        // create a new thread to accept the next connection
		System.out.println("=> WebServer.run()");
        try {
            DataOutputStream out=
                new DataOutputStream(socket.getOutputStream());

            try {
                try {
                    String[] request= readRequestLines(socket.getInputStream()); 
					System.out.println("REQUEST:");
                    for(int i = 0; i < request.length; i++){
                    	System.out.println("\t" + i + " : " + request[i]);
                    }
                                       
                    String method= getMethod(request);
                    String type= getType(request);
                    byte[] body= getBody(request);

                    System.out.println(
                        Thread.currentThread().getName()
                            + " > HTTP/1.0 200 OK");

                    out.writeBytes("HTTP/1.0 200 OK\r\n");
                    out.writeBytes("Content-Length: " + body.length + "\r\n");
                    out.writeBytes("Content-Type: " + type + "\r\n");
                    out.writeBytes("\r\n");

                    if (!method.equals("HEAD")) {
                    	// Den Inhalt schreiben
                    	out.writeBytes(header.getHeader());
                        out.write(body, 0, body.length);
                    }
                } catch (HTTPException e) {
                    String method= getMethod(e.requestLine);
                    File file= null;
                    String type= "text/html";
                    byte[] body= createDefaultDocument(e);

                    System.out.println(
                        Thread.currentThread().getName()
                            + " > HTTP/1.0 "
                            + e.statusCode
                            + " "
                            + e.reasonPhrase);

                    out.writeBytes(
                        "HTTP/1.0 "
                            + e.statusCode
                            + " "
                            + e.reasonPhrase
                            + "\r\n");
                    out.writeBytes("Content-Length: " + body.length + "\r\n");
                    out.writeBytes("Content-Type: " + type + "\r\n");
                    out.writeBytes("\r\n");

                    if (!method.equals("HEAD")) {
                        out.write(body, 0, body.length);
                    }
                }
            } catch (IOException e) {
				e.printStackTrace();
                System.out.println(
                    Thread.currentThread().getName()
                        + " run() > HTTP/1.0 500 Internal Server Error" + e.getMessage());
                out.writeBytes("HTTP/1.0 500 Internal Server Error\r\n\r\n");
            } finally {
                out.flush();
            }
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            try {
                socket.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        System.out.println("<= WebServer.run()");
    }

    /**
     * Liest den HTTP Cleint Request und gibt ein String Array dafür zurück.
     * @param http client request
     * @return string array representation
     * @throws IOException
     */
    private String[] readRequestLines(InputStream in) throws IOException {
		System.out.println("=> WebServer.readRequestLines()");
        BufferedReader br= new BufferedReader(new InputStreamReader(in));
        String s= br.readLine();
        String[] v= s.split(" ", 3);

        for (int i= 0; i < v.length; ++i) {
            v[i]= URLDecoder.decode(v[i], "UTF-8");
        }

        while (s.length() > 0) { // eat the rest of header
            System.out.println(Thread.currentThread().getName() + " < " + s);
            s= br.readLine();
        }
		System.out.println("<= WebServer.readRequestLines() > " + v);
        return v;		
    }

    /**
     * Gibt die HTTP Methode zurück, entweder GET oder HEAD
     * @param request
     * @return http method
     * @throws HTTPException 501
     */
    private String getMethod(String[] request) throws HTTPException {
		System.out.println("=> WebServer.getMethod(String[] request)");
		String method= request[0];
		System.out.println("<= WebServer.getMethod(String[] request) > " + method);
        if (method.equals("GET") || method.equals("HEAD")) {
            return method;			
        } else {
			System.out.println(
				 Thread.currentThread().getName()
					 + " getMethod() > " + method + " 501 Not Implemented");
            throw new HTTPException("501", "Not Implemented", request);
        }
    }

    /**
     * Nimmt die 2. Zeile des Request und versucht die dort angegebene Datei 
     * als <code>File</code> zu laden.
     * @param request
     * @return requested file
     * @throws HTTPException 404 or 403
     */
    private File getFile(String[] request) throws HTTPException {
		System.out.println("=> WebServer.getFile(String[] request)");
		String filename = root + request[1].replace('/', File.separatorChar);	
        File file= new File(filename);

        if (!file.exists()) {
			System.out.println(
				 Thread.currentThread().getName()
					 + " getFile() > " + filename + " 404 Not Found");             	
            throw new HTTPException("404", "Not Found", request);
        }

        if (file.isDirectory() || file.isHidden()) {
			System.out.println(
				 Thread.currentThread().getName()
					 + " getFile() > " + file + " 403 Forbidden");        	
            throw new HTTPException("403", "Forbidden", request);
        }
		System.out.println("<= WebServer.getFile(String[] request) > " + file);
        return file;
    }

    /**
     * Gibt den MimeType abhängig von der Endung der
     * angeforderten Datei zurück.
     * @param request
     * @return mimi type
     * @throws HTTPException 415
     */
    private String getType(String[] request) throws HTTPException {
		System.out.println("=> WebServer.getType(String[] request)");
		String path= request[1];
		int actionpoint  = path.indexOf("?");
		if(actionpoint > 0){
			// Löschen der Paramter
			path = path.substring(0, actionpoint);
			System.out.println("request gekürzt > " + path);
		}	
        String type=
            (String) mime_types.get(path.substring(path.lastIndexOf('.') + 1));
		System.out.println("<= WebServer.getType(String[] request) > " + type);
        if (type != null) {
            return type;
        } else {
			System.out.println(
				 Thread.currentThread().getName()
					 + " getType() > " + type + " 415 Unsupported Media Type");             	
            throw new HTTPException("415", "Unsupported Media Type", request);
        }
    }

    /**
     * Nimmt die 2. Zeile des Request und gibt entweder die dort angegebene Datei
     * oder wenn ein "?" enthalten ist die Rückgabe der entsprechenden Aktion 
     * im Binärfomat zurück.
     * @param request
     * @return file content
     * @throws HTTPException 500 or 404 or 403
     */
    private byte[] getBody(String[] request) throws HTTPException, RemoteException {
		System.out.println("=> WebServer.getBody(String[] request)");	
		String url = root + request[1].replace('/', File.separatorChar);	
		// testet ob ein "?" enthalten ist => Parameter parsen
		int actionpoint  = url.indexOf("?");
		if(actionpoint > 0){
			// TODO parsen der Attribute
			String urlpath = url.substring(0, actionpoint);
			String actions = url.substring(actionpoint + 1);			
			System.out.println("Aufruf von actionString > " + actions );
			String[] propStrings= actions.split("&", 0);
			Hashtable requestProps = new Hashtable();
			for(int i = 0; i< propStrings.length; i++){
				String tempProp = propStrings[i];
				int delim = tempProp.indexOf("=");
				String key = tempProp.substring(0, delim);
				String value = tempProp.substring(delim+1);
				requestProps.put(key, value);
				System.out.println("add prop > key : " + key +" | value : " + value);
			}
			System.out.println("props" + propStrings);
			return getActionBody(request, requestProps);
		} else {
			return getFileBody(request);
		}
    }

    /**
     * Gibt den Content zurück, wenn eine Aktion aufgerufen wird und muß von jedem
     * Server entsprechend bearbeitet (überschrieben) werden.
     * @param request
     * @return
     */
    protected abstract byte[] getActionBody(String[] request, Hashtable requestProps) throws RemoteException ;

    private byte[] getFileBody(String[] request) throws HTTPException {
        File file= getFile(request); // 404 or 403
        System.out.println("\tDatei "+ file + "wird gelesen");
        try {
            byte[] body= new byte[(int) file.length()];
        
            System.out.println(
                Thread.currentThread().getName()
                    + " | "
                    + file.getAbsolutePath()
                    + " ("
                    + file.length()
                    + " bytes)");
            new DataInputStream(new FileInputStream(file)).readFully(body);
        	System.out.println("<= WebServer.getBody(String[] request)" + body);
            return body;
        } catch (IOException e) {
        	System.out.println(
        		 Thread.currentThread().getName()
        			 + " getBody() > " + file + " 500 Internal Server Error");           	
            throw new HTTPException("500", "Internal Server Error", request);
        }
    }

    /**
     * Creates an entity to be included in an error response.
     * 
     * @param e the exception
     * @return an array of bytes for the default document
     * @throws UnsupportedEncodingException if an internal server error occurred
     * @throws UnknownHostException if an internal server error occurred
     */
    protected byte[] createDefaultDocument(HTTPException e)
        throws UnsupportedEncodingException, UnknownHostException {
        return (
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                + "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML Basic 1.0//EN\" \"http://www.w3.org/TR/xhtml-basic/xhtml-basic10.dtd\">\n"
                + "<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en\">\n"
                + "<head>\n"
                + "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" />\n"
                + "<title>"
                + e.statusCode
                + " "
                + e.reasonPhrase
                + "</title>\n"
                + "</head>\n"
                + "<body>\n"
                + "<h1>"
                + e.statusCode
                + " "
                + e.reasonPhrase
                + "</h1>\n"
                + "<dl>\n"
                + "<dt>Requested Method</dt><dd>"
                + e.requestLine[0]
                + "</dd>\n"
                + "<dt>Requested URI</dt><dd>"
                + e.requestLine[1]
                + "</dd>\n"
                + "<dt>Requested Version</dt><dd>"
                + e.requestLine[2]
                + "</dd>\n"
                + "</dl>\n"
                + "<address>Simple Web Server ver.0.01 on "
                + InetAddress.getLocalHost().getHostName()
                + "</address>\n"
                + "</body>\n"
                + "</html>\n").getBytes(
            "UTF-8");
    }
}
