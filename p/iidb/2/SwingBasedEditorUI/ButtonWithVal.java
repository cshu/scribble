package griddoor.SwingBasedEditorUI;

public class ButtonWithVal<T> extends Button {

	public T val;
	public ButtonWithVal(ComponentGroup cg, T val) {
		super(cg);
		this.val = val;
	}

	@Override
	public TextComponent cloneCom(ComponentGroup newcg) {
		return new ButtonWithVal<T>(newcg, val);
	}

	@Override
	public void fillByPrepending() {
		fillEditorWithLines(val.toString()+"\n");
	}

}
