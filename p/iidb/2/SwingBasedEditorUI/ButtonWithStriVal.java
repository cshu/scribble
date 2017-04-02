package griddoor.SwingBasedEditorUI;

public class ButtonWithStriVal extends Button {

	public String strival;
	public ButtonWithStriVal(ComponentGroup cg, String strival) {
		super(cg);
		this.strival = strival;
	}
	@Override
	public void fillByPrepending() {
		fillEditorWithLines(strival+'\n');
	}
	@Override
	public ButtonWithStriVal cloneCom(ComponentGroup newcg) {
		return new ButtonWithStriVal(newcg, strival);
	}
}
