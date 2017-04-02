package griddoor.SwingBasedEditorUI;

import java.util.Iterator;

public class SelectWithMutatingSrc<T> extends Selectable {
	Iterable<T> src;
	
	public SelectWithMutatingSrc(String descLabel, ComponentGroup cg, Iterable<T> src) {
		super(cg);
		this.descLabel = descLabel;
		this.src = src;
	}
	
	final String descLabel;

	@Override
	public TextComponent cloneCom(ComponentGroup newcg) {
		return new SelectWithMutatingSrc<>(this.descLabel, newcg, this.src);
	}

	public static final String tyname = "SelectWMS`";
	public T selectedItem;
	
	@Override
	public void fillByPrepending() {
		fillEditorWithLines(tyname+descLabel+'`'+selectedItem+"\n");
	}
	
	@Override
	public void spacePressed() {
		Iterator<T> ite = src.iterator();
		if(ite.hasNext()){
			this.cg.modalD = new ComponentGroupForSelWMS<>(this);
			do{
				T val = ite.next();
				if(val.equals(selectedItem))
					this.cg.modalD.cursorIndex = this.cg.modalD.components.size();
				this.cg.modalD.components.add(new ButtonWithVal<T>(this.cg.modalD, val));
			}while(ite.hasNext());
		}else{
			showMsgBox("No items");
		}
		this.cg.context.redrawNeeded = true;
	}

}
