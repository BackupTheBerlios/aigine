/*
 * Created on 19.05.2004
 *
 */
package projects.voting.control;

import java.awt.Component;

import javax.swing.JComponent;
import javax.swing.JTable;
import javax.swing.table.TableCellRenderer;

/**
 *
 * @author danny
 * @since 19.05.2004 17:28:02
 */
public class JComponentCellRenderer implements TableCellRenderer
{
	/**
	 * Gibt den TableCellRenderer zurück.
	 * @see javax.swing.table.TableCellRenderer#getTableCellRendererComponent(javax.swing.JTable, java.lang.Object, boolean, boolean, int, int)
	 */
	public Component getTableCellRendererComponent(JTable table, Object value, boolean isSelected, boolean hasFocus, int row, int column) {
		return (JComponent)value;
	}
}
