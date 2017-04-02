package griddoor.SwingBasedEditorUI;

public class CheckBoxWithLabel extends CheckBox {
	public String label;
	public CheckBoxWithLabel(ComponentGroup cg, String label) {
		super(cg);
		this.label = label;
	}
	@Override
	public TextComponent cloneCom(ComponentGroup newcg) {
		return new CheckBoxWithLabel(newcg, label);//undone checked not copied?
	}
	@Override
	public void fillByPrepending() {
		this.fillEditorWithLines((checked ? "CHECKED   " : "Unchecked ") + label);
	}

}
