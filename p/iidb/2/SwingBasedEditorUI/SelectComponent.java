package griddoor.SwingBasedEditorUI;


public class SelectComponent<T> extends Selectable {

	public static final String tyname = "SelectCom`";
	
	public ComponentGroupForSelect<T> selectionCg;
	final String descLabel;
	public SelectComponent(String descLabel, ComponentGroup cg, T[] optionsText) {
		super(cg);
		this.descLabel = descLabel;
		selectionCg = new ComponentGroupForSelect<T>(this);
		for(T optText : optionsText){
			selectionCg.components.add(new ButtonWithVal<T>(selectionCg, optText));
		}
	}
	public void addOpt(T opt){
		selectionCg.components.add(new ButtonWithVal<T>(selectionCg, opt));
	}
	private SelectComponent(String descLabel, ComponentGroup cg){
		super(cg);
		this.descLabel = descLabel;
	}
	
	@Override
	public void fillByPrepending() {
		fillEditorWithLines(tyname+descLabel+'`'+((ButtonWithVal<?>)this.selectionCg.getSelectedCom()).val.toString()+"\n");
	}
	@Override
	public void spacePressed() {
		cg.modalD = selectionCg.cloneGroForSameCg();
		this.cg.context.redrawNeeded = true;
	}
	public void selectOptFunc(){
	}

	@Override
	public TextComponent cloneCom(ComponentGroup newcg) {
		SelectComponent<T> valtoret = new SelectComponent<T>(descLabel, newcg);
		valtoret.selectionCg = selectionCg.cloneForNewCg(valtoret);
		return valtoret;
	}
}
