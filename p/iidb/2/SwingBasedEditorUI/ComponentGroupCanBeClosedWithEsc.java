package griddoor.SwingBasedEditorUI;

import java.util.ArrayList;

public class ComponentGroupCanBeClosedWithEsc extends ComponentGroup {

	public ComponentGroupCanBeClosedWithEsc(ComponentGroup cg) {
		super(cg);
	}
	public ComponentGroupCanBeClosedWithEsc(ComponentGroup cg, ArrayList<TextComponent> coms) {
		super(cg,coms);
	} 
	
	@Override
	public void escPressed() {
		if(modalD==null){
			this.closeItself();
			this.context.redrawNeeded = true;
		}else
			modalD.escPressed();
	}

	@Override
	public ComponentGroup cloneGroForSameCg() {
		ComponentGroupCanBeClosedWithEsc valtoret = new ComponentGroupCanBeClosedWithEsc(cg, new ArrayList<TextComponent>(components.size()));
		for(TextComponent com : this.components){
			valtoret.components.add(com.cloneCom(valtoret));
		}
		valtoret.cursorIndex = cursorIndex;
		return valtoret;
	}
}
