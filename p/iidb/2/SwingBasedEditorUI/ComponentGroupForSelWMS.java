package griddoor.SwingBasedEditorUI;

public class ComponentGroupForSelWMS<T> extends ComponentGroupCanBeClosedWithEsc {

	SelectWithMutatingSrc<T> sc;
	public ComponentGroupForSelWMS(SelectWithMutatingSrc<T> sc) {
		super(sc.cg);
		this.sc = sc;
	}

	@SuppressWarnings("unchecked")
	@Override
	public void spacePressed() {
		closeItself();
		sc.selectedItem = ((ButtonWithVal<T>)this.getSelectedCom()).val;
		this.cg.context.redrawNeeded = true;
	}
}
