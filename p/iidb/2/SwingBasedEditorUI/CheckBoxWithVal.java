package griddoor.SwingBasedEditorUI;

public class CheckBoxWithVal<T> extends CheckBox {

	public T val;
	public String label;
	public CheckBoxWithVal(ComponentGroup cg, T val, String label) {
		super(cg);
		this.val = val;
		this.label = label;
	}
	@Override
	public TextComponent cloneCom(ComponentGroup newcg) {
		return new CheckBoxWithVal<T>(newcg, val, label);//undone checked not copied?
	}

	@Override
	public void fillByPrepending() {
		this.fillEditorWithLines((checked ? "CHECKED   " : "Unchecked ") + label);
	}
}
