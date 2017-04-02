package griddoor.SwingBasedEditorUI;

public class CloseButton extends ButtonWithFixedText {

	public CloseButton(ComponentGroup cg, String fixedText) {
		super(cg, fixedText);
	}

	@Override
	public void spacePressed() {
		cg.closeItself();
		this.cg.context.redrawNeeded = true;
	}
	
	@Override
	public TextComponent cloneCom(ComponentGroup newcg) {
		return new CloseButton(newcg,fixedText);
	}
}
