/*
 * Created on 19.05.2004
 *
 */
package projects.voting.control;

import java.awt.FontMetrics;

import javax.swing.JComponent;
import javax.swing.JTable;
import javax.swing.table.TableCellEditor;
import javax.swing.table.TableCellRenderer;
import javax.swing.table.TableColumn;
import javax.swing.table.TableModel;

/**
 *
 * @author danny
 * @since 19.05.2004 17:32:47
 */
public class JVoteTable extends JTable {
	public TableCellRenderer getCellRenderer(int row, int column) {
		TableColumn tableColumn = getColumnModel().getColumn(column);
		TableCellRenderer renderer = tableColumn.getCellRenderer();
		if (renderer == null) {
			Class c = getColumnClass(column);
			if (c.equals(Object.class)) {
				Object o = getValueAt(row, column);
				if (o != null)
					c = getValueAt(row, column).getClass();
			}
			renderer = getDefaultRenderer(c);
		}
		return renderer;
	}

	public TableCellEditor getCellEditor(int row, int column) {
		TableColumn tableColumn = getColumnModel().getColumn(column);
		TableCellEditor editor = tableColumn.getCellEditor();
		if (editor == null) {
			Class c = getColumnClass(column);
			if (c.equals(Object.class)) {
				Object o = getValueAt(row, column);
				if (o != null)
					c = getValueAt(row, column).getClass();
			}
			editor = getDefaultEditor(c);
		}
		return editor;
	}

	/**
	 * Adjusts the widths of a JTable columns
	 * @param  table    the table
	 */
	public void resizeTableColumns() {
		this.setAutoResizeMode(JTable.AUTO_RESIZE_OFF);
		TableColumn c = null;
		FontMetrics fm = this.getFontMetrics(this.getFont());
		for (int i = 0; i < this.getColumnCount(); i++) {
			c = this.getColumn(this.getColumnName(i));
			c.setPreferredWidth(determineColumnWidth(c, this.getModel(), fm));
		}
	}

	/**
	 * Returns the preferred width of a JTable column. The resulting width
	 * is 5 pixels wider than the biggest column
	 * @param  col    the column
	 * @param  model  the table model
	 * @param  fm     the font metrics
	 */
	public int determineColumnWidth(
		TableColumn col,
		TableModel model,
		FontMetrics fm) {
		String value = (String) col.getHeaderValue();
		int headerWidth = fm.stringWidth("." + value.trim() + ".");
		int columnNumber = col.getModelIndex();
		int max = headerWidth;
		int columnWidth = 0;
		int nrows = model.getRowCount();
		String cell = "";
		for (int i = 0; i < nrows; i++) {
			try {
				JComponent c = (JComponent) model.getValueAt(i, columnNumber);
				columnWidth = c.getWidth();
			} catch (ClassCastException cce) {
				value = model.getValueAt(i, columnNumber).toString();
				columnWidth = fm.stringWidth("." + value.trim() + ".");
			}
			if (columnWidth > max)
				max = columnWidth;
		}
		return max + 5;
	}
}
