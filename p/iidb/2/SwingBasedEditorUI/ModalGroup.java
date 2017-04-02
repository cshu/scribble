package griddoor.SwingBasedEditorUI;

public class ModalGroup extends ButtonWithFixedText {

	public static final String tyname = "ModalGroup`";
	
	public ComponentGroupAffiliatedToModalGro savedModal;
	public ModalGroup(ComponentGroup cg, String fixedText) {
		super(cg, fixedText);
	}
	
	@Override
	public void spacePressed() {
		cg.modalD = savedModal.cloneGroForSameCg();
		cg.context.redrawNeeded = true;
	}
	
	@Override
	public TextComponent cloneCom(ComponentGroup newcg) {
		ModalGroup valtoret = new ModalGroup(newcg,fixedText);
		valtoret.savedModal = savedModal.cloneForNewCg(valtoret);
		return valtoret;
	}
	

}
