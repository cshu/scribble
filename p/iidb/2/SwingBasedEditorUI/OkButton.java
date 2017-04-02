package griddoor.SwingBasedEditorUI;

public class OkButton extends ButtonWithFixedText {

	public OkButton(ComponentGroup cg) {
		super(cg, "OK");
	}
	public OkButton(ComponentGroup cg, String fixedText) {
		super(cg, fixedText);
	}
	
	@Override
	public void spacePressed() {
		ComponentGroupAffiliatedToModalGro thisCg = ((ComponentGroupAffiliatedToModalGro)cg);
		thisCg.modGro.savedModal = thisCg;
		cg.closeItself();
		this.cg.context.redrawNeeded = true;
	}

	@Override
	public TextComponent cloneCom(ComponentGroup newcg) {
		return new OkButton(newcg,fixedText);
	}
}
