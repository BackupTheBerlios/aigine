/*
 * Created on 18.03.2005
 *
 * @user drichter
 * */
package projects.catalog.db;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Properties;

import projects.catalog.model.PictureDTO;

import API.model.AbstractDAO;
import API.model.AbstractDTO;

/**
 * @author drichter
 *
 * 
 */
public class PictureDAO extends AbstractDAO {

	/* (non-Javadoc)
	 * @see API.model.AbstractDAO#getTableName()
	 */
	protected String getTableName() {
		return "t_pictures" ;
	}

	/* (non-Javadoc)
	 * @see API.model.AbstractDAO#getRowNames()
	 */
	protected String getRowNames() {
		return "ID, titel, comment, created, upload" ;
	}
	
	/**
	 * erzeugt DTO-Objektarray anhand des gegeben ResultSets
	 * @param rs
	 * @return
	 * @throws SQLException
	 */
	private PictureDTO[] generateDTO(ResultSet rs) throws SQLException {
		ArrayList theDTOs = new ArrayList() ;
		while (rs.next()) {
			theDTOs.add(new PictureDTO(rs.getString("titel"), rs.getString("comment"), rs.getDate("created"), rs.getDate("upload"))) ;
		}
		if (theDTOs.size() < 1) {
			return null ;
		} else {
			return (PictureDTO[]) theDTOs.toArray(new PictureDTO[theDTOs.size()]) ;
		}
	}

	/* (non-Javadoc)
	 * @see API.model.AbstractDAO#insert()
	 */
	public void insert() {
		// TODO Auto-generated method stub

	}

	/* (non-Javadoc)
	 * @see API.model.AbstractDAO#update()
	 */
	public void update() {
		// TODO Auto-generated method stub

	}

	/* (non-Javadoc)
	 * @see API.model.AbstractDAO#load(java.util.Properties)
	 */
	public AbstractDTO load(Properties parameter) throws SQLException {
		ResultSet rs = this.basicLoad(parameter) ;
		return (AbstractDTO) (this.generateDTO(rs))[0] ;
	}

	/* (non-Javadoc)
	 * @see API.model.AbstractDAO#loadList()
	 */
	public AbstractDTO[] loadList(Properties parameter) throws SQLException {
		ResultSet rs = this.basicLoad(parameter) ;
		return (AbstractDTO[]) (this.generateDTO(rs)) ;
	}

	/* (non-Javadoc)
	 * @see API.model.AbstractDAO#translateVarToCol(java.lang.String)
	 */
	protected String translateVarToCol(String var) {
		if (var.equals("titel"))
			return "titel" ;
		else if (var.equals("comment"))
			return "comment" ;
		else if (var.equals("created"))
			return "created" ;
		else if (var.equals("upload"))
			return "upload" ;
		else
			return "FEHLER" ;
	}

	/* (non-Javadoc)
	 * @see API.model.AbstractDAO#generateSubSelect(java.util.Properties)
	 */
	protected String generateSubSelect(Properties props) {
		StringBuffer sb = new StringBuffer("") ;
		if (props.containsKey("categoryID_SUB")) {
			sb.append("(SELECT picture_ID FROM at_pic_at_cats WHERE cats_ID IN (SELECT ID FROM AT_SUBCAT_AT_CAT WHERE category_ID = " + props.getProperty("category_ID_SUB")) ;
			if (props.containsKey("subcategoryID_SUB")) {
				sb.append(" and subcat_ID = " + props.getProperty("subcategoryID_SUB")) ;
			}
			sb.append(");") ;
		}
		return sb.toString() ;
	}

	/* (non-Javadoc)
	 * @see API.model.AbstractDAO#insert(API.model.AbstractDTO)
	 */
	public void insert(AbstractDTO theDTO) throws SQLException {
		// TODO Auto-generated method stub
		
	}

	/* (non-Javadoc)
	 * @see API.model.AbstractDAO#update(API.model.AbstractDTO)
	 */
	public void update(AbstractDTO theDTO) throws SQLException {
		// TODO Auto-generated method stub
		
	}

}
