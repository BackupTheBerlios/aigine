package projects.voting.model;

import java.io.Serializable;

/**
 * Speichert die für ein Vote benötigten Daten.
 * @author danny
 * @since 05.05.2004 21:04:31
 */
public class Vote implements Serializable{
	private long id;
	private int count = 0;
	private String description = new String("");
	
	public Vote(){ }
	
	public String toString(){
		return new String("\tcount: " + this.getCount() + 
						" \tdescription: " + this.getDescription()
						);
	}
	
	public Object[] toArray(){
		Object[] os = new Object[2];
		os[0] = Integer.toString(this.getCount());
		os[1] = this.getDescription();
		System.out.println("aufruf toArray() => " + os[0] + " | " + os[1]);
		return os;
	}
	
	/**
	 * @return count
	 */
	public int getCount() {
		return count;
	}
	
	/**
	 * @return count
	 */
	public Integer getCountI() {
		return new Integer(count);
	}

	/**
	 * @return description
	 */
	public String getDescription() {
		return description;
	}

	/**
	 * @param count
	 */
	public void setCount(int count) {
		this.count = count;
	}

	/**
	 * @param string
	 */
	public void setDescription(String description) {
		this.description = description;
	}

	/**
	 * @return
	 * Vote.java
	 */
	public long getId() {
		return id;
	}

	/**
	 * keep the id-handling to hibernate to use them witch 
	 * may access property get- and set-Methods of all  visibilities
	 * @param l long variable with a unique id
	 * Vote.java
	 */
	private void setId(long l) {
		id = l;
	}

}
