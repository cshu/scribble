package griddoor.SwingBasedEditorUI;

import java.util.ArrayList;

public class ComponentGroupAffiliatedToModalGro extends ComponentGroup {

	public final ModalGroup modGro;
	public ComponentGroupAffiliatedToModalGro(ModalGroup mg) {
		super(mg.cg);
		modGro = mg;
	}
	public ComponentGroupAffiliatedToModalGro(ModalGroup mg, ArrayList<TextComponent> coms) {
		super(mg.cg, coms);
		modGro = mg;
	}

	@Override
	public ComponentGroupAffiliatedToModalGro cloneGroForSameCg() {
		return cloneForNewCg(modGro);
	}
	public ComponentGroupAffiliatedToModalGro cloneForNewCg(ModalGroup newmg){
		ComponentGroupAffiliatedToModalGro valtoret = new ComponentGroupAffiliatedToModalGro(newmg, new ArrayList<TextComponent>(components.size()));
		for(TextComponent com : this.components){
			valtoret.components.add(com.cloneCom(valtoret));
		}
		valtoret.cursorIndex = cursorIndex;
		return valtoret;
	}
}
