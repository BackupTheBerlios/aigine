/*
 * Created on 07.09.2004
 *
 */
package projects.catalog.model;

/**
 * @author your mama
 *
 * Subcatalogs are catalogs which spezialice the thems
 */
public class Category {
	private long id;
	private String name;
	private Catalog mainCatalog;

	/**
	 * @return Returns the id.
	 */
	public long getId() {
		return id;
	}
	/**
	 * @param id The id to set.
	 */
	public void setId(long id) {
		this.id = id;
	}
	/**
	 * @return Returns the mainCatalog.
	 */
	public Catalog getMainCatalog() {
		return mainCatalog;
	}
	/**
	 * @param mainCatalog The mainCatalog to set.
	 */
	public void setMainCatalog(Catalog mainCatalog) {
		this.mainCatalog = mainCatalog;
	}
	/**
	 * @return Returns the name.
	 */
	public String getName() {
		return name;
	}
	/**
	 * @param name The name to set.
	 */
	public void setName(String name) {
		this.name = name;
	}
}
