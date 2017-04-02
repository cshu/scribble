package griddoor.SwingBasedEditorUI;

import java.util.ArrayList;

public class ComponentGroupForSelect<T> extends ComponentGroupCanBeClosedWithEsc {
	SelectComponent<T> sc;
	public ComponentGroupForSelect(SelectComponent<T> sc) {
		super(sc.cg);
		this.sc = sc;
	}
	public ComponentGroupForSelect(SelectComponent<T> sc, ArrayList<TextComponent> coms) {
		super(sc.cg,coms);
		this.sc = sc;
	}

	@Override
	public void spacePressed() {
		closeItself();
		sc.selectionCg = this;
		this.cg.context.redrawNeeded = true;
	}
	
	@Override
	public ComponentGroupForSelect<T> cloneGroForSameCg(){
		return cloneForNewCg(sc);
	}
	public ComponentGroupForSelect<T> cloneForNewCg(SelectComponent<T> newsc){
		ComponentGroupForSelect<T> valtoret = new ComponentGroupForSelect<T>(newsc, new ArrayList<TextComponent>(this.components.size()));
		for(TextComponent com : this.components){
			valtoret.components.add(com.cloneCom(valtoret));
		}
		valtoret.cursorIndex = this.cursorIndex;
		return valtoret;
	}
}
