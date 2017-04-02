package griddoor.SwingBasedEditorUI;

public abstract class CheckBox extends Selectable {
	
	public static final String tyname = "Check`";

	public boolean checked;
	
	public CheckBox(ComponentGroup cg) {
		super(cg);
	}

	@Override
	public void spacePressed(){
		checked = !checked;
		this.cg.context.redrawNeeded = true;
	}
}
